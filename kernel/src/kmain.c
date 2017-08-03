#include <arch/x86/interrupts.h>
#include <assert.h>
#include <drivers/serial.h>
#include <drivers/vga.h>
#include <kernel/logger.h>
#include <multiboot2.h>

void kmain(void *info, unsigned int eax);

// TODO: Think about a way simpler / generic / automatic way
static void init_system() {

  // Init arch
  serial_init(SERIAL_COM1); // Init first, for debugging purpose
  init_idt();

  // Plateform stuff
  vga_init();

  // Memory management
  // Drivers
}

void kmain(void *info, unsigned int eax) {
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
