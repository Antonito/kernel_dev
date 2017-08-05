#include <arch/x86/interrupts.h>
#include <arch/x86/registers.h>
#include <assert.h>
#include <drivers/ata.h>
#include <drivers/serial.h>
#include <drivers/vga.h>
#include <kernel/logger.h>
#include <kernel/pmm.h>
#include <multiboot2.h>

void kmain(void *info, unsigned int eax);

// TODO: Think about a way simpler / generic / automatic way
static void init_system() {

  // Init arch
  serial_init(SERIAL_COM1); // Init first, for debugging purpose
  init_idt();
  init_sse();

  // Plateform stuff
  vga_init();

  // Memory management
  pmm_init();

  // Drivers
  ata_init();
}

void kmain(void *info, unsigned int eax) {
  (void)info; // TODO: Use multiboot informations
  if (eax != MULTIBOOT2_BOOTLOADER_MAGIC) {
    return;
  }

  init_system();
  LOG(LOG_INFO, "System booted\n\r");
  g_klogger.set_output(LOG_GRAPHIC | LOG_SERIAL);
  LOG(LOG_INFO, "Welcome to BordOS !\n\r");

  for (;;) {
    __asm__("hlt;");
  }
}
