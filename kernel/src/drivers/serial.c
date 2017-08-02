
#include <arch/x86/io.h>
#include <assert.h>
#include <drivers/serial.h>
#include <string.h>

//
// Private functions
//
static int32_t serial_received(uint16_t const port);
static int32_t serial_is_transmit_empty(uint16_t const port);

// Read datas from a serial port
static int32_t serial_received(uint16_t const port) {
  return inb(port + 5) & 1;
}

static int32_t serial_is_transmit_empty(uint16_t const port) {
  return inb(port + 5) & 0x20;
}

//
// Public functions
//

void serial_init(uint16_t const port) {
  outb(port + 1, 0x00); // Disable all interrupts
  outb(port + 3, 0x80); // Enable DLAB (set baud rate divisor)
  outb(port + 0, 0x03); // Set divisor to 3 (lo byte) 38400 baud
  outb(port + 1, 0x00); //                  (hi byte)
  outb(port + 3, 0x03); // 8 bits, no parity, one stop bit
  outb(port + 2, 0xC7); // Enable FIFO, clear them, with 14-byte threshold
  outb(port + 4, 0x0B); // IRQs enabled, RTS/DSR set
}

// Read a uint8_t
uint8_t serial_read(uint16_t const port) {
  while (!serial_received(port))
    ;
  return inb(port);
}

// Write a uint8_t
void serial_put(uint16_t const port, uint8_t const a) {
  while (!serial_is_transmit_empty(port))
    ;
  outb(port, a);
}

// Write a signed number
void serial_write_nb_signed(uint16_t const port, int32_t nb,
                            const uint8_t base) {
  assert(base == 10);
  if (nb < 0) {
    serial_put(port, '-');
    nb *= -1;
  }
  serial_write_nb(port, (uint32_t)nb, base);
}

// Write a number
void serial_write_nb(uint16_t const port, uint32_t const nb,
                     const uint8_t base) {
  static char const *const _base = "0123456789ABCDEF";

  assert(base <= 16 && "Base should be <= 16");
  if (nb > base) {
    serial_write_nb(port, nb / base, base);
  }
  // Ouput the character
  serial_put(port, (uint8_t)_base[nb % base]);
}

// Writes a NUL-terminated string
void serial_write_str(uint16_t const port, char const *str) {
  assert(str);

  while (*str) {
    serial_put(port, (uint8_t)*str);
    if (*str == '\n') {
      serial_put(port, (uint8_t)'\r');
    }
    ++str;
  }

  // Active waiting
  while (!serial_is_transmit_empty(port))
    ;
}

// Write a buffer on port
void serial_write(uint16_t const port, void const *str, size_t len) {
  uint8_t const *data = str;
  while (len) {
    serial_put(port, *data);
    ++data;
    --len;
  }

  // Active waiting
  while (!serial_is_transmit_empty(port))
    ;
}

//
// Variadic print
//

int32_t serial_vprintf(char const *restrict format, va_list ap) {
  size_t ite = 0;

  // TODO: Improve perf (don't write char by char ? :p ), support more
  // formating options

  assert(format && "Format string and va list cannot be nullptr");
  while (format[ite]) {
    if (format[ite] == '%' && format[ite + 1]) {
      ++ite;

      switch (format[ite]) {
      case 's': {
        char const *_tmp = va_arg(ap, char *);
        serial_write_str(SERIAL_COM1, _tmp);
      } break;
      case 'd': {
        int32_t _nb = va_arg(ap, int32_t);
        serial_write_nb_signed(SERIAL_COM1, _nb, 10);
      } break;
      case 'u': {
        uint32_t _nbu = va_arg(ap, uint32_t);
        serial_write_nb(SERIAL_COM1, _nbu, 10);
      } break;
      case 'x': {
        uint32_t _nbX = va_arg(ap, uint32_t);
        serial_write_nb(SERIAL_COM1, _nbX, 16);
      } break;
      default:
        assert(0 && "Not supported format string option");
        break;
      }

    } else {
      serial_put(SERIAL_COM1, (uint8_t)format[ite]);
    }

    ++ite;
  }
  return (int32_t)ite;
}

int32_t serial_printf(char const *restrict format, ...) {
  va_list ap;

  va_start(ap, format);
  int32_t ret = serial_vprintf(format, ap);
  va_end(ap);
  return ret;
}
