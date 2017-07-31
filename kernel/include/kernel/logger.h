#pragma once

#include <stdint.h>

enum kernel_logger_level_e {
  LOG_DEBUG = 0,
  LOG_INFO,
  LOG_WARNING,
  LOG_ERROR,
  LOG_CRITICAL,
  NB_LOG_LEVEL
};

enum kernel_logger_output_e {
  LOG_SERIAL = 0x1, // 0b00000001
  LOG_GRAPHIC = 0x3 // 0b00000010
};

#if defined KERNEL_NO_LOG
#define LOG(...)
#else
#define LOG(fmt, ...) g_klogger.log(g_klogger.level, fmt, ##__VA_ARGS__)
#endif

struct kernel_logger_t {
  int32_t (*log)(enum kernel_logger_level_e const lvl, char const *const fmt,
                 ...);
  void (*set_level)(enum kernel_logger_level_e const level);
  void (*set_output)(uint8_t const output);
  uint8_t output;
  enum kernel_logger_level_e level;
};

extern struct kernel_logger_t g_klogger;
