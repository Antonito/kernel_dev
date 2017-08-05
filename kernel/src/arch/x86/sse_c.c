#include <arch/x86/registers.h>
#include <kernel/logger.h>

extern int check_sse();

void init_sse(void) {
  if (check_sse()) {
    LOG(LOG_INFO, "SSE activated.\r\n");
  }
}
