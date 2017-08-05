#include <arch/x86/registers.h>
#include <kernel/logger.h>

extern int check_sse();
extern int check_sse2();
extern int check_sse3();
extern int check_ssse3();
extern int check_sse41();
extern int check_sse42();
extern int check_sse4A();

void init_sse(void) {
  // Check SSE and activate it if supported
  if (check_sse()) {
    LOG(LOG_INFO, "SSE activated.\r\n");
  }

  // Check SSE2 support
  if (check_sse2()) {
    LOG(LOG_INFO, "SSE2 supported\r\n");
  }

  // Check SSE3 support
  if (check_sse3()) {
    LOG(LOG_INFO, "SSE3 supported\r\n");
  }

  // Check SSSE3 support
  if (check_ssse3()) {
    LOG(LOG_INFO, "SSSE3 supported\r\n");
  }

  // Check SSE4.1 support
  if (check_sse41()) {
    LOG(LOG_INFO, "SSE4.1 supported\r\n");
  }

  // Check SSE4.2 support
  if (check_sse42()) {
    LOG(LOG_INFO, "SSE4.2 supported\r\n");
  }

  // Check SSE4A support
  if (check_sse4A()) {
    LOG(LOG_INFO, "SSE4A supported\r\n");
  }
}
