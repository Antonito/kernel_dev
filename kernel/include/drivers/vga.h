#pragma once

#include <stdint.h>

enum vga_color_e {
  VGA_BLACK = 0,
  VGA_BLUE,
  VGA_GREEN,
  VGA_CYAN,
  VGA_RED,
  VGA_MAGENTA,
  VGA_BROWN,
  VGA_LIGHT_GREY,
  VGA_DARK_GREY,
  VGA_LIGHT_BLUE,
  VGA_LIGHT_GREEN,
  VGA_LIGHT_CYAN,
  VGA_LIGHT_RED,
  VGA_LIGHT_MAGENTA,
  VGA_LIGHT_BROWN,
  VGA_WHITE
};

void vga_init(void);

void vga_setcolor(enum vga_color_e const fg, enum vga_color_e const bg);
void vga_put(uint8_t const c);
void vga_write_str(char const *str);
void vga_write_nb_signed(int32_t nb, const uint8_t base);
void vga_write_nb(uint32_t const nb, const uint8_t base);
void vga_clear(void);
