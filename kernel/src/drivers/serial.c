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
  assert(port == SERIAL_COM1 || port == SERIAL_COM2 || port == SERIAL_COM3 ||
         port == SERIAL_COM4);
  return inb(port + 5) & 1;
}

static int32_t serial_is_transmit_empty(uint16_t const port) {
  assert(port == SERIAL_COM1 || port == SERIAL_COM2 || port == SERIAL_COM3 ||
         port == SERIAL_COM4);
  return inb(port + 5) & 0x20;
}

//
// Public functions
//

void serial_init(uint16_t const port) {
  assert(port == SERIAL_COM1 || port == SERIAL_COM2 || port == SERIAL_COM3 ||
         port == SERIAL_COM4);

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
  assert(port == SERIAL_COM1 || port == SERIAL_COM2 || port == SERIAL_COM3 ||
         port == SERIAL_COM4);
  while (!serial_received(port))
    ;
  return inb(port);
}

// Write a uint8_t
void serial_put(uint16_t const port, uint8_t const a) {
  assert(port == SERIAL_COM1 || port == SERIAL_COM2 || port == SERIAL_COM3 ||
         port == SERIAL_COM4);
  while (!serial_is_transmit_empty(port))
    ;
  outb(port, a);
}

// Write a number
void serial_write_nb(uint16_t const port, const uint32_t nb,
                     const uint8_t base) {
  static char const *const _base = "0123456789ABCDEF";

  assert(port == SERIAL_COM1 || port == SERIAL_COM2 || port == SERIAL_COM3 ||
         port == SERIAL_COM4);
  assert(base <= 16 && "Base should be <= 16");

  if (nb > base) {
    serial_write_nb(port, nb / base, base);
  }
  // Ouput the character
  serial_put(port, (uint8_t)_base[nb % base]);
}

// Writes a NUL-terminated string
void serial_write_str(uint16_t const port, const char *str) {
  assert(str);
  assert(port == SERIAL_COM1 || port == SERIAL_COM2 || port == SERIAL_COM3 ||
         port == SERIAL_COM4);
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
  assert(port == SERIAL_COM1 || port == SERIAL_COM2 || port == SERIAL_COM3 ||
         port == SERIAL_COM4);

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

int32_t serial_vprintf(const char *restrict format, va_list ap) {
  // TODO: Implement
  serial_write_str(SERIAL_COM1, format);
  return (int32_t)strlen(format);
}
