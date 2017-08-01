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
void vga_putchar();
void vga_write();
void vga_clear(void);
