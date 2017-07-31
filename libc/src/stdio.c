#include <stdarg.h>
#include <stdio.h>

int vprintf(char const *restrict format, va_list ap) {
  (void)format, (void)ap;
  // TODO: Implement, VGA output
  return 0;
}

int printf(char const *restrict format, ...) {
  va_list ap;

  va_start(ap, format);
  int ret = vprintf(format, ap);
  va_end(ap);
  return ret;
}
