#pragma once

#include <stdint.h>

typedef struct s_cmos_rtc cmos_rtc_t;

struct s_cmos_rtc {
  uint8_t seconds;
  uint8_t minuts;
  uint8_t hour;
  uint8_t day;
  uint8_t month;
  uint16_t year;
  uint8_t century;
};

void cmos_RTC(cmos_rtc_t *const data);

void cmos_read(uint8_t _128arr[]);
void cmos_write(uint8_t _128arr[]);
