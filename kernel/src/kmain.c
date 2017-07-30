#include <multiboot.h>

void kmain(multiboot_info_t *info, unsigned int eax);

void kmain(multiboot_info_t *info, unsigned int eax) {
  if (eax != MULTIBOOT_BOOTLOADER_MAGIC) {
    return;
  }

  for (;;) {
    __asm__("hlt;");
  }
}
