void kmain(void *info, unsigned int eax) {
  if (eax != 0xBADB002) {
    return;
  }

  for (;;) {
    __asm__("hlt;");
  }
}
