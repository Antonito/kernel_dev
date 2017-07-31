#pragma once

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

// Serial ports
enum serial_port_e {
  SERIAL_COM1 = 0x3F8,
  SERIAL_COM2 = 0x2F8,
  SERIAL_COM3 = 0x3E8,
  SERIAL_COM4 = 0x2E8
};

// Initialize one serial port
void serial_init(uint16_t const port);

// Output functions
void serial_put(uint16_t const port, uint8_t const a);
void serial_write_str(uint16_t const port, char const *str);
void serial_write(uint16_t const port, void const *str, size_t len);
void serial_write_nb(uint16_t const port, uint32_t const nb,
                     uint8_t const base);
int32_t serial_vprintf(char const *restrict format, va_list ap);
int32_t serial_printf(char const *restrict format, ...);

// Input functions
uint8_t serial_read(uint16_t const port);
