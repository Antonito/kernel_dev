#include <arch/x86/io.h>
#include <assert.h>
#include <drivers/cmos.h>

#define CMOS_REG (0x70)
#define CMOS_DATA (0x71)

#define CURRENT_YEAR (2017)

/* Doc: http://www.bioscentral.com/misc/cmosmap.htm */
enum CMOS_VALUES {
  CMOS_RTC_SEC = 0x00,
  CMOS_RTC_SEC_ALARM = 0x01,
  CMOS_RTC_MIN = 0x02,
  CMOS_RTC_MIN_ALARM = 0x03,
  CMOS_RTC_HOUR = 0x04,
  CMOS_RTC_HOUR_ALARM = 0x05,
  CMOS_RTC_DAY_OF_WEEK = 0x06,
  CMOS_RTC_DATE_DAY = 0x07,
  CMOS_RTC_DATE_MONTH = 0x08,
  CMOS_RTC_DATE_YEAR = 0x09,
  CMOS_STAT_REG_A = 0x0A,
  CMOS_STAT_REG_B = 0x0B,
  CMOS_STAT_REG_C = 0x0C,
  CMOS_STAT_REG_D = 0x0D,
  CMOS_DIAGNOSTIC = 0x0E,
  CMOS_SHUTDOWN_STAT = 0x0F,
  CMOS_FLOPPY_DD_TYPE,
  CMOS_SYS_CONF_SETTINGS,
  CMOS_HARD_DISK_TYPE,
  CMOS_TYPEMATIC_PARAM,
  CMOS_INSTALLED_EQUIP,
  CMOS_LOW_MEM_ORD_BYTE,
  CMOS_HIGH_MEM_ORD_BYTE,
  CMOS_EXT_LOW_ORD_BYTE,
  CMOS_EXT_HIGH_ORD_BYTE,
  CMOS_HARD_DISK_0_EXT_TYPE,
  CMOS_HARD_DISK_1_EXT_TYPE,
  CMOS_USR_DEF_C_NB_CYLINDER_LSB,
  CMOS_USR_DEF_C_NB_CYLINDER_MSB,
  CMOS_USR_DEF_C_NB_HEAD,
  CMOS_USR_DEF_C_WRITE_PRECOMP_CYLINDER_LSB,
  CMOS_USR_DEF_C_WRITE_PRECOMP_CYLINDER_MSB,
  CMOS_USR_DEF_C_CR_BYTE,
  CMOS_USR_DEF_C_LANDING_ZONE_LSB,
  CMOS_USR_DEF_C_LANDING_ZONE_MSB,
  CMOS_USR_DEF_C_NB_SECTORS,
  CMOS_USR_DEF_D_NB_CYLINDER_LSB,
  CMOS_USR_DEF_D_NB_CYLINDER_MSB,
  CMOS_USR_DEF_D_NB_HEAD,
  CMOS_USR_DEF_D_WRITE_PRECOMP_CYLINDER_LSB,
  CMOS_USR_DEF_D_WRITE_PRECOMP_CYLINDER_MSB,
  CMOS_USR_DEF_D_CR_BYTE,
  CMOS_USR_DEF_D_LANDING_ZONE_LSB,
  CMOS_USR_DEF_D_LANDING_ZONE_MSB,
  CMOS_USR_DEF_D_NB_SECTORS,
  CMOS_SYSTEM_OPE_FLAG,
  CMOS_CMOS_CHKSUM_HIGH_ORD_BYTE,
  CMOS_CMOS_CHKSUM_LOW_ORD_BYTE,
  CMOS_ACTUAL_EXT_MEM_LOW_ORD_BYTE,
  CMOS_ACTUAL_EXT_MEM_HIGH_ORD_BYTE,
  CMOS_CENTURY_DATE_BCD,
  CMOS_POST_INFO_FLAG,
  CMOS_BIOS_N_SHADOW_OPT_FLAG1,
  CMOS_BIOS_N_SHADOW_OPT_FLAG2,
  CMOS_CHIPSET_SPECIFIC_INFO,
  CMOS_PASSW_SEED_AND_COLOR_OPT,
  CMOS_ENCRYPT_PASSWD,
  CMOS_EXT_CMOS_CHKSUM_MSB,
  CMOS_EXT_CMOS_CHKSUM_LSB,
  CMOS_MODEL_NB_BYTE,
  CMOS_SERIAL_NB_BYTE_1,
  CMOS_SERIAL_NB_BYTE_2,
  CMOS_SERIAL_NB_BYTE_3,
  CMOS_SERIAL_NB_BYTE_4,
  CMOS_SERIAL_NB_BYTE_5,
  CMOS_SERIAL_NB_BYTE_6,
  CMOS_CRC_BYTE,
  CMOS_CENTURY_BYTE,
  CMOS_DATE_ALARM,
  CMOS_EXTENDED_CR_4A,
  CMOS_EXTENDED_CR_4B,
  CMOS_RSV1,
  CMOS_RSV2,
  CMOS_RTC_ADDR2,
  CMOS_RTC_ADDR3,
  CMOS_EXT_RAM_LSB,
  CMOS_EXT_RAM_MSB,
  CMOS_RSV3,
  CMOS_EXT_RAM_DATA_PORT,
  CMOS_RSV4,
  CMOS_RSV5,
  CMOS_RSV6,
  CMOS_RSV7,
  CMOS_RSV8,
  CMOS_RSV9,
  CMOS_RSV10,
  CMOS_RSV11,
  CMOS_RSV12,
  CMOS_RSV13
};

/* Read a value from CMOS */
static inline uint8_t cmos_read_raw(uint8_t reg) {
  outb(CMOS_REG, reg);
  return inb(CMOS_DATA);
}

/* Read the whole CMOS */
void cmos_read(uint8_t _128arr[]) {
  for (uint8_t i = 0; i < 128; ++i) {
    __asm__("cli;");
    _128arr[i] = cmos_read_raw(i);
    __asm__("sti;");
  }
}

/* Writes a value to the CMOS */
static inline void cmos_write_raw(uint8_t reg, uint8_t val) {
  outb(CMOS_REG, reg);
  outb(CMOS_DATA, val);
}

/* Completly rewrite the CMOS */
void cmos_write(uint8_t _128arr[]) {
  for (uint8_t i = 0; i < 128; ++i) {
    __asm__("cli;");
    cmos_write_raw(i, _128arr[i]);
    __asm__("sti;");
  }
}

static int8_t cmos_get_update() {
  return (int8_t)(cmos_read_raw(CMOS_STAT_REG_A) & 0x80);
}

static void __cmos_fill_rtc(cmos_rtc_t *const data) {
  while (cmos_get_update())
    ;
  data->seconds = cmos_read_raw(CMOS_RTC_SEC);
  data->minuts = cmos_read_raw(CMOS_RTC_MIN);
  data->hour = cmos_read_raw(CMOS_RTC_HOUR);
  data->day = cmos_read_raw(CMOS_RTC_DATE_DAY);
  data->month = cmos_read_raw(CMOS_RTC_DATE_MONTH);
  data->year = cmos_read_raw(CMOS_RTC_DATE_YEAR);
  data->century = 0;
}

#define BCD2BIN(val) ((val & 0x0F) + ((val / 16) * 10))

void cmos_RTC(cmos_rtc_t *const data) {
  cmos_rtc_t last = {0, 0, 0, 0, 0, 0, 0};
  uint8_t registerB;

  assert(data && "Should not be null.");
  __cmos_fill_rtc(data);

  /* Avoid incosistent values */
  do {
    last.seconds = data->seconds;
    last.minuts = data->minuts;
    last.hour = data->hour;
    last.day = data->day;
    last.month = data->month;
    last.year = data->year;
    last.century = data->century;
    __cmos_fill_rtc(data);
  } while (last.seconds != data->seconds && last.minuts != data->minuts &&
           last.hour != data->hour && last.day != data->day &&
           last.month != data->month && last.year != data->year &&
           last.century != data->century);
  registerB = cmos_read_raw(CMOS_STAT_REG_B);

  /* Convert BCD to binary, if needed */
  if (!(registerB & 0x4)) {
    data->seconds = BCD2BIN(data->seconds);
    data->minuts = BCD2BIN(data->minuts);
    data->hour = ((data->hour & 0x0F) + (((data->hour & 0x70) / 16) * 10)) |
                 (data->hour & 0x80);
    data->day = BCD2BIN(data->day);
    data->month = BCD2BIN(data->month);
    data->year = BCD2BIN(data->year);
    data->century = BCD2BIN(data->century);
  }

  /* Convert clock to 24h format, if needed */
  if (!(registerB & 0x02) && (data->hour & 0x80)) {
    data->hour = ((data->hour & 0x7F) + 12) % 24;
  }

  /* Calculate the 4-digit year */
  if (data->century != 0) /* In fact, you check the register */
  {
    ;
  } else {
    data->year += (CURRENT_YEAR / 100) * 100;
    if (data->year > CURRENT_YEAR) {
      data->year += 100;
    }
  }
}
