#include <arch/x86/interrupts.h>
#include <arch/x86/io.h>
#include <arch/x86/utils.h>
#include <assert.h>
#include <drivers/ata.h>
#include <drivers/device.h>
#include <kernel/logger.h>
#include <stddef.h>
#include <string.h>

// Doc:
// https://github.com/levex/osdev/blob/bba025f8cfced6ad1addc625aaf9dab8fa7aef80/drivers/ata.c
// http://isse.cqu.edu.cn/svn/epos/kernel/ide.c
// http://isse.cqu.edu.cn/svn/epos/kernel/

static void delay400ns(uint16_t bus);
static int32_t ata_wait(uint16_t bus, int32_t advanced);
static void ata_wait_ready(uint16_t bus);
static void ata_select(uint16_t bus);
static void ide_init(uint16_t bus);
static void ata_primary_irq(struct s_regs *reg);
static void ata_secondary_irq(struct s_regs *reg);
static void ide_read_sector(uint16_t bus, uint8_t slave, uint32_t lba,
                            uint8_t *buf);
static void ide_write_sector(uint16_t bus, uint8_t slave, uint32_t lba,
                             uint8_t *buf);
static uint8_t ata_read(uint8_t *buffer, uint32_t offset, uint32_t len,
                        void *dev);
static uint8_t ata_write(uint8_t *buffer, uint32_t offset, uint32_t len,
                         void *dev);

static void delay400ns(uint16_t bus) {
  inb(bus + ATA_REG_ALTSTATUS);
  inb(bus + ATA_REG_ALTSTATUS);
  inb(bus + ATA_REG_ALTSTATUS);
  inb(bus + ATA_REG_ALTSTATUS);
}

static int32_t ata_wait(uint16_t bus, int32_t advanced) {
  uint8_t status = 0;

  delay400ns(bus);
  while ((status = inb(bus + ATA_REG_STATUS)) & ATA_SR_BSY)
    ;

  if (advanced) {
    status = inb(bus + ATA_REG_STATUS);
    if (status & ATA_SR_ERR) {
      return 1;
    }
    if (status & ATA_SR_DF) {
      return 1;
    }
    if (!(status & ATA_SR_DRQ)) {
      return 1;
    }
  }
  return 0;
}

static void ata_wait_ready(uint16_t bus) {
  while (inb(bus + ATA_REG_STATUS) & ATA_SR_BSY)
    ;
}

static void ata_select(uint16_t bus) { outb(bus + ATA_REG_HDDEVSEL, 0xA0); }

static void ide_init(uint16_t bus) {
  int i;
  uint16_t *buf;
  uint8_t *ptr;

  outb(bus + 1, 1);
  outb(bus + 0x306, 0);

  ata_select(bus);
  delay400ns(bus);

  outb(bus + ATA_REG_COMMAND, ATA_CMD_IDENTIFY);
  delay400ns(bus);

  inb(bus + ATA_REG_COMMAND);

  ata_wait_ready(bus);

  ata_identify_t device; // = kcalloc(sizeof(device)); // TODO: Alloc
  memset(&device, 0, sizeof(device));
  buf = (uint16_t *)&device;

  for (i = 0; i < 256; ++i) {
    buf[i] = inw(bus);
  }

  ptr = (uint8_t *)&device.model;
  for (i = 0; i < 39; i += 2) {
    uint8_t tmp = ptr[i + 1];
    ptr[i + 1] = ptr[i];
    ptr[i] = tmp;
  }

  outb(bus + ATA_REG_CONTROL, 0x02);
  if (device.flags) {
    char serial[sizeof(device.serial) + 1];
    char firmware[sizeof(device.firmware) + 1];
    char model[sizeof(device.model) + 1];

    memcpy(serial, device.serial, sizeof(device.serial));
    memcpy(firmware, device.firmware, sizeof(device.firmware));
    memcpy(model, device.model, sizeof(device.model));
    serial[sizeof(device.serial)] = '\0';
    firmware[sizeof(device.firmware)] = '\0';
    model[sizeof(device.model)] = '\0';

    LOG(LOG_INFO,
        "ATA:\n\rFlags: %x\n\rSerial: %s\n\rFirmware: %s\n\rModel: %s\n\r",
        device.flags, serial, firmware, model);
    // TODO: Don't use hardcoded values
    struct device_t dev = {"Primary disk", 32,         DEVICE_BLOCK, NULL,
                           &ata_read,      &ata_write, &device};
    (void)dev;
  }
  // else { kfree(device); } // TODO: Alloc
}

static void ide_read_sector(uint16_t bus, uint8_t slave, uint32_t lba,
                            uint8_t *buf) {
  outb(bus + ATA_REG_CONTROL, 0x02);

  ata_wait_ready(bus);

  outb(bus + ATA_REG_HDDEVSEL,
       ((uint8_t)(0xe0 | slave << 4) | (lba & 0x0f000000) >> 24));
  outb(bus + ATA_REG_FEATURES, 0x00);
  outb(bus + ATA_REG_SECCOUNT0, 1);
  outb(bus + ATA_REG_LBA0, (uint8_t)((lba & 0x000000ff) >> 0));
  outb(bus + ATA_REG_LBA1, (uint8_t)((lba & 0x0000ff00) >> 8));
  outb(bus + ATA_REG_LBA2, (uint8_t)((lba & 0x00ff0000) >> 16));
  outb(bus + ATA_REG_COMMAND, ATA_CMD_READ_PIO);

  if (ata_wait(bus, 1)) {
    LOG(LOG_ERROR, "Error ATA reading bus=0x%03x, %s, lba=%d", bus,
        slave ? "slave" : "master", lba);
  }

  repinsw(bus, buf, 256);
  ata_wait(bus, 0);
}

static void ide_write_sector(uint16_t bus, uint8_t slave, uint32_t lba,
                             uint8_t *buf) {
  outb(bus + ATA_REG_CONTROL, 0x02);

  ata_wait_ready(bus);

  outb(bus + ATA_REG_HDDEVSEL,
       (uint8_t)(0xe0 | slave << 4) | (lba & 0x0f000000) >> 24);
  ata_wait(bus, 0);
  outb(bus + ATA_REG_FEATURES, 0x00);
  outb(bus + ATA_REG_SECCOUNT0, 0x01);
  outb(bus + ATA_REG_LBA0, (lba & 0x000000ff) >> 0);
  outb(bus + ATA_REG_LBA1, (lba & 0x0000ff00) >> 8);
  outb(bus + ATA_REG_LBA2, (lba & 0x00ff0000) >> 16);
  outb(bus + ATA_REG_COMMAND, ATA_CMD_WRITE_PIO);
  ata_wait(bus, 0);

  repoutsw(bus, buf, 256);
  outb(bus + ATA_REG_COMMAND, ATA_CMD_CACHE_FLUSH);
  ata_wait(bus, 0);
}

static void ata_primary_irq(struct s_regs *reg) { (void)reg; }

static void ata_secondary_irq(struct s_regs *reg) { (void)reg; }

static uint8_t ata_read(uint8_t *buffer, uint32_t offset, uint32_t len,
                        void *dev) {
  (void)offset, (void)len;
  if (!buffer || !dev) {
    return 0;
  }
  return 1;
}

// TODO: Code write and read ATA
static uint8_t ata_write(uint8_t *buffer, uint32_t offset, uint32_t len,
                         void *dev) {
  (void)offset, (void)len;
  if (!buffer || !dev) {
    return 0;
  }
  return 1;
}

#if 0
static void ata_probe(void) {
  LOG(LOG_INFO, "Probing ATA drives\r\n");
  // Check for primary master disk
  if (ide_find(ATA_PRIMARY, ATA_MASTER)) {
    LOG(LOG_INFO, "Found ATA primary master\r\n");

    // Allocate private datas
    ide_device_t *priv = kcalloc(sizeof(*priv));
    assert(priv);
    priv->drive = (ATA_PRIMARY << 1) | ATA_MASTER;

    // Create device
    struct device_t dev = {"Primary disk", 32,         DEVICE_BLOCK, NULL,
                           &ata_read,      &ata_write, priv};

    // Add device to device manager
    device_add(&dev);
  }
}
#endif

void ata_init(void) {
  LOG(LOG_INFO, "Initializing ATA driver\r\n");
  irq_set_routine(14, &ata_primary_irq);
  irq_set_routine(15, &ata_secondary_irq);
  ide_init(ATA_PRIMARY_IO);
  ide_init(ATA_SECONDARY_IO);
  (void)ide_write_sector;
  (void)ide_read_sector;
}
