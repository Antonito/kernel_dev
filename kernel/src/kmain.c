#include <assert.h>
#include <drivers/serial.h>
#include <drivers/vga.h>
#include <kernel/logger.h>
#include <multiboot.h>

void kmain(multiboot_info_t *info, unsigned int eax);

// TODO: Think about a way simpler / generic / automatic way
static void init_system() {
  serial_init(SERIAL_COM1);
  vga_init();
}

void kmain(multiboot_info_t *info, unsigned int eax) {
  if (eax != MULTIBOOT_BOOTLOADER_MAGIC) {
    return;
  }

  init_system();
  LOG(LOG_INFO, "System booted\n\r");
  g_klogger.set_output(LOG_GRAPHIC);
  LOG(LOG_INFO, "Welcome to BordOS !\n\r");
  for (;;) {
    __asm__("hlt;");
  }
}
