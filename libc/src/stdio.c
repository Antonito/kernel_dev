#include <assert.h>
#include <private/kernel.h>
#include <stdarg.h>
#include <stdio.h>

int vprintf(char const *restrict format, va_list ap) {
  size_t ite = 0;

  // TODO: Improve perf (don't write char by char ? :p ), support more
  // formating options

  assert(format && ap && "Format string and va list cannot be nullptr");
  while (format[ite]) {
    if (format[ite] == '%' && format[ite + 1]) {
      ++ite;

      switch (format[ite]) {
      case 's': {
        char const *_tmp = va_arg(ap, char *);
        vga_write_str(_tmp);
      } break;
      case 'd': {
        int32_t _nb = va_arg(ap, int32_t);
        vga_write_nb_signed(_nb, 10);
      } break;
      case 'u': {
        uint32_t _nbu = va_arg(ap, uint32_t);
        vga_write_nb(_nbu, 10);
      } break;
      case 'x': {
        uint32_t _nbX = va_arg(ap, uint32_t);
        vga_write_nb(_nbX, 16);
      } break;
      default:
        assert(0 && "Not supported format string option");
        break;
      }

    } else {
      vga_put((uint8_t)format[ite]);
    }

    ++ite;
  }
  return (int32_t)ite;
}

int printf(char const *restrict format, ...) {
  va_list ap;

  va_start(ap, format);
  int ret = vprintf(format, ap);
  va_end(ap);
  return ret;
}
