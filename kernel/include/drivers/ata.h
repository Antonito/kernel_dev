#pragma once

#include <stdint.h>

// Status code
enum ata_sr_e {
  ATA_SR_ERR = 1,                  // Error
  ATA_SR_IDX = (ATA_SR_ERR << 1),  // Inlex
  ATA_SR_CORR = (ATA_SR_ERR << 2), // Corrected data
  ATA_SR_DRQ = (ATA_SR_ERR << 3),  // Data request ready
  ATA_SR_DSC = (ATA_SR_ERR << 4),  // Drive seek complete
  ATA_SR_DF = (ATA_SR_ERR << 5),   // Drive write fault
  ATA_SR_DRDY = (ATA_SR_ERR << 6), // Drive ready
  ATA_SR_BSY = (ATA_SR_ERR << 7)   // Busy
};

// Error code
enum ata_er_e {
  ATA_ER_AMNF = 1,                   // No address mark
  ATA_ER_TK0NF = (ATA_ER_AMNF << 1), // Track 0 not found
  ATA_ER_ABRT = (ATA_ER_AMNF << 2),  // Command aborted
  ATA_ER_MCR = (ATA_ER_AMNF << 3),   // No media
  ATA_ER_IDNF = (ATA_ER_AMNF << 4),  // ID Mark not found
  ATA_ER_MC = (ATA_ER_AMNF << 5),    // No media
  ATA_ER_UNC = (ATA_ER_AMNF << 6),   // Uncorrectable data
  ATA_ER_BBK = (ATA_ER_AMNF << 7)    // Bad sector
};

// ATA commands
enum ata_cmd_e {
  ATA_CMD_READ_PIO = 0x20,
  ATA_CMD_READ_PIO_EXT = 0x24,
  ATA_CMD_READ_DMA = 0xC8,
  ATA_CMD_READ_DMA_EXT = 0x25,
  ATA_CMD_WRITE_PIO = 0x30,
  ATA_CMD_WRITE_PIO_EXT = 0x34,
  ATA_CMD_WRITE_DMA = 0xCA,
  ATA_CMD_WRITE_DMA_EXT = 0x35,
  ATA_CMD_CACHE_FLUSH = 0xE7,
  ATA_CMD_CACHE_FLUSH_EXT = 0xEA,
  ATA_CMD_PACKET = 0xA0,
  ATA_CMD_IDENTIFY_PACKET = 0xA1,
  ATA_CMD_IDENTIFY = 0xEC,
  ATAPI_CMD_READ = 0xA8,
  ATAPI_CMD_EJECT = 0x1B
};

// ATA identify
enum ata_ident_e {
  ATA_IDENT_DEVICETYPE = 0,
  ATA_IDENT_CYLINDERS = 2,
  ATA_IDENT_HEADS = 6,
  ATA_IDENT_SECTORS = 12,
  ATA_IDENT_SERIAL = 20,
  ATA_IDENT_MODEL = 54,
  ATA_IDENT_CAPABILITIES = 98,
  ATA_IDENT_FIELDVALID = 106,
  ATA_IDENT_MAX_LBA = 120,
  ATA_IDENT_COMMANDSETS = 164,
  ATA_IDENT_MAX_LBA_EXT = 200
};

enum ide_type_e { IDE_ATA = 0x00, IDE_ATAPI = 0x01 };

enum ata_drive_e { ATA_MASTER = 0x00, ATA_SLAVE = 0x01 };

#define ATA_REG_DATA 0x00
#define ATA_REG_ERROR 0x01
#define ATA_REG_FEATURES 0x01
#define ATA_REG_SECCOUNT0 0x02
#define ATA_REG_LBA0 0x03
#define ATA_REG_LBA1 0x04
#define ATA_REG_LBA2 0x05
#define ATA_REG_HDDEVSEL 0x06
#define ATA_REG_COMMAND 0x07
#define ATA_REG_STATUS 0x07
#define ATA_REG_SECCOUNT1 0x08
#define ATA_REG_LBA3 0x09
#define ATA_REG_LBA4 0x0A
#define ATA_REG_LBA5 0x0B
#define ATA_REG_CONTROL 0x0C
#define ATA_REG_ALTSTATUS 0x0C
#define ATA_REG_DEVADDRESS 0x0D

// Channels
enum ata_chan_e { ATA_PRIMARY = 0x00, ATA_SECONDARY = 0x01 };

// Directions
enum ata_dir_e { ATA_READ = 0x00, ATA_WRITE = 0x13 };

typedef struct ide_device {
  uint8_t reserved;      // 0: Empty, 1: Really exists
  uint8_t channel;       // 0: Primary, 1: Secondary (enum ata_chan_e)
  uint8_t drive;         // 0: Master, 1: Slave (enum ata_drive_e)
  uint16_t type;         // 0: ATA, 1: ATAPI (enum ide_type_e)
  uint16_t signature;    // Drive signature
  uint16_t capabilities; // Features
  uint32_t command_sets; // Commad sets supported
  uint32_t size;         // Size in sectors
  uint8_t model[41];     // Model in string
} ide_device_t;

typedef struct ata_identify {
  uint16_t flags;
  uint16_t unused1[9];
  char serial[20];
  uint16_t unused2[3];
  char firmware[8];
  char model[40];
  uint16_t sectors_per_int;
  uint16_t unused3;
  uint16_t capabilities[2];
  uint16_t unused4[2];
  uint16_t valid_ext_data;
  uint16_t unused5[5];
  uint16_t size_of_rw_mult;
  uint32_t sectors_28;
  uint16_t unused6[38];
  uint64_t sectors_48;
  uint16_t unused7[152];
} __attribute__((packed)) ata_identify_t;

void ata_init(void);
