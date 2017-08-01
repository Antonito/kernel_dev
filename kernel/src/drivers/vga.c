#include <drivers/vga.h>
#include <kernel/logger.h>
#include <string.h>

// Global definitions
static int32_t const vga_width = 80;
static int32_t const vga_height = 25;
static uint16_t *const vga_buffer = (uint16_t *)((uintptr_t)0xB8000);

// Struct definition
struct vga_data_t {
  uint8_t x;
  uint8_t y;
  uint8_t attr;
  uint8_t __padding;
  uint16_t buff[vga_width * vga_height];
};

static struct vga_data_t vga_data = {0, 0, 0, 0, {0}};

void vga_setcolor(enum vga_color_e const fg, enum vga_color_e const bg) {
  vga_data.attr = (uint8_t)((fg & 0x0F) | (bg << 4));
}

void vga_clear(void) {
  uint16_t blank = 0x20 | (vga_data.attr);

  // Fill buffer
  for (int32_t i = 0; i < vga_width * vga_height; ++i) {
    vga_data.buff[i] = blank;
  }

  // Upadte buffers
  memcpy(vga_buffer, vga_data.buff, vga_width * vga_height * sizeof(uint16_t));

  // Update cursor
  vga_data.x = 0;
  vga_data.y = 0;
  // vga_move_cursor();
}

void vga_init(void) {
  // LOG(LOG_INFO, "Initializing VGA screen\n\r");
  vga_clear();
}
