#include <arch/x86/io.h>
#include <assert.h>
#include <drivers/vga.h>
#include <kernel/logger.h>
#include <string.h>

#define VGA_WIDTH (80)
#define VGA_HEIGHT (25)

// Global definitions
static int32_t const vga_width = VGA_WIDTH;
static int32_t const vga_height = VGA_HEIGHT;
static uint16_t *const vga_buffer = (uint16_t *)((uintptr_t)0xB8000);

// Struct definition
struct vga_data_t {
  uint8_t x;
  uint8_t y;
  uint8_t attr;
  uint8_t __padding;
  uint16_t buff[VGA_WIDTH * VGA_WIDTH];
};

#undef VGA_WIDTH
#undef VGA_HEIGHT

static void vga_scroll(void);
static void vga_move_cursor(void);

static struct vga_data_t vga_data = {0, 0, 0, 0, {0}};

static void vga_scroll(void) {
  uint16_t blank = (uint16_t)(0x20 | (vga_data.attr << 8));

  // If row is 25, we need to scroll up
  if (vga_data.y >= vga_height) {
    // Move the current line one line back in the buffer
    uint32_t temp = vga_data.y - vga_height + 1;

    memcpy(vga_buffer, vga_buffer + temp * vga_width,
           (vga_height - temp) * vga_width * 2);

    // Clear the last line
    for (int32_t i = 0; i < vga_width; ++i) {
      *(vga_buffer + (vga_height - temp) * vga_width + i) = blank;
    }
    vga_data.y = vga_height - 1;
  }
}

static void vga_move_cursor(void) {
  uint32_t const temp = vga_data.y * vga_width + vga_data.x;

  outb(0x3D4, 14);
  outb(0x3D5, (uint8_t)(temp >> 8));
  outb(0x3D4, 15);
  outb(0x3D5, temp & 0xFF);
}

void vga_setcolor(enum vga_color_e const fg, enum vga_color_e const bg) {
  vga_data.attr = (uint8_t)((fg & 0x0F) | (bg << 4));
}

void vga_clear(void) {
  // Update buffer
  for (int32_t i = 0; i < vga_height; ++i) {
    for (int32_t j = 0; j < vga_width; ++j) {
      uint16_t const blank = 0x20 | (vga_data.attr);

      vga_buffer[i * vga_width + j] = blank;
    }
  }

  // Update cursor
  vga_data.x = 0;
  vga_data.y = 0;
  vga_move_cursor();
}

void vga_put(uint8_t const c) {
  // Handle backspace
  if (c == '\b') {
    if (vga_data.x != 0) {
      --vga_data.x;
    }
  }

  // Increment x to a point that will make it divisible by 8
  else if (c == '\t') {
    vga_data.x = (vga_data.x + 8) & ~(8 - 1);
  }

  // Bring cursor back to the margin
  else if (c == '\r') {
    vga_data.x = 0;
  }

  // Handle new line just like in DOS and BIOS
  else if (c == '\n') {
    vga_data.x = 0;
    ++vga_data.y;
  }

  // Any character >= ' ' is printable
  else if (c >= ' ') {
    uint16_t *const where = &vga_buffer[vga_data.y * vga_width + vga_data.x];
    *where = ((uint16_t)c | (uint16_t)(vga_data.attr << 8));
    ++vga_data.x;
  }

  // We reached the end, new line
  if (vga_data.x >= vga_width) {
    vga_data.x = 0;
    ++vga_data.y;
  }

  vga_scroll();
  vga_move_cursor();
}

// Write a signed number
void vga_write_nb_signed(int32_t nb, const uint8_t base) {
  assert(base == 10);
  if (nb < 0) {
    vga_put('-');
    nb *= -1;
  }
  vga_write_nb((uint32_t)nb, base);
}

// Write a number
void vga_write_nb(uint32_t const nb, const uint8_t base) {
  static char const *const _base = "0123456789ABCDEF";
  assert(base <= 16 && "Base should be <= 16");

  if (nb > base) {
    vga_write_nb(nb / base, base);
  }
  // Ouput the character
  vga_put((uint8_t)_base[nb % base]);
}

void vga_write_str(char const *str) {
  assert(str);
  while (*str) {
    vga_put((uint8_t)*str);
    ++str;
  }
}

void vga_init(void) {
  LOG(LOG_INFO, "Initializing VGA screen\n\r");
  vga_clear();
  vga_setcolor(VGA_LIGHT_GREY, VGA_BLACK);
}
