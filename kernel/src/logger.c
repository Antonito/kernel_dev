#include <assert.h>
#include <drivers/serial.h>
#include <kernel/logger.h>
#include <stdarg.h>
#include <stdio.h>

// Prototypes
static int32_t kernel_logger_log(enum kernel_logger_level_e const lvl,
                                 char const *const fmt, ...);
static void kernel_logger_set_level(enum kernel_logger_level_e const level);
static void kernel_logger_set_output(uint8_t const output);
static int32_t kernel_logger_print_header(uint8_t const output,
                                          enum kernel_logger_level_e const lvl);
static int32_t kernel_logger_print_msg(uint8_t const output,
                                       enum kernel_logger_level_e const lvl,
                                       char const *const fmt, va_list ap);

// Logger declaration
struct kernel_logger_t g_klogger = {&kernel_logger_log,
                                    &kernel_logger_set_level,
                                    &kernel_logger_set_output, LOG_SERIAL,
#if defined DEBUG
                                    LOG_DEBUG
#else
                                    LOG_INFO
#endif
};

static int32_t kernel_logger_log(enum kernel_logger_level_e const lvl,
                                 char const *const fmt, ...) {
  va_list ap;
  int32_t ret = 0;

  assert(g_klogger.level < NB_LOG_LEVEL);
  if (lvl >= g_klogger.level) {
    va_start(ap, fmt);
    ret = kernel_logger_print_msg(g_klogger.output, lvl, fmt, ap);
    va_end(ap);
  }
  return ret;
}

static void kernel_logger_set_level(enum kernel_logger_level_e const level) {
  assert(level >= LOG_DEBUG && level <= LOG_CRITICAL);
  g_klogger.level = level;
}

static void kernel_logger_set_output(uint8_t const output) {
  assert(output == LOG_SERIAL || output == LOG_GRAPHIC ||
         output == (LOG_SERIAL | LOG_GRAPHIC));
  g_klogger.output = output;
}

//
// Internals
//

static int32_t
kernel_logger_print_header(uint8_t const output,
                           enum kernel_logger_level_e const lvl) {
  int32_t ret = 0;

  // Print log header
  if (output & LOG_SERIAL) {
    // Output on serial port
  }
  if (output & LOG_GRAPHIC) {
    // Output on screen display
  }
  return ret;
}

static int32_t kernel_logger_print_msg(uint8_t const output,
                                       enum kernel_logger_level_e const lvl,
                                       char const *const fmt, va_list ap) {
  int32_t ret = kernel_logger_print_header(output, lvl);
  if (output & LOG_SERIAL) {
    // Output on serial port
    ret += serial_vprintf(fmt, ap);
  }
  if (output & LOG_GRAPHIC) {
    // Output on screen display
    ret += vprintf(fmt, ap);
  }
  return ret;
}