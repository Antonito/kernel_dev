#include <private/kernel.h>
#include <stdlib.h>

// Prototype
__attribute__((noreturn)) void
abort_handler(char const *const msg, char const *const file, int32_t line);

// Abort the system. Shall never return
__attribute__((noreturn)) void
abort_handler(char const *const msg, char const *const file, int32_t line) {
  // Disable interrupts
  __asm__("cli;");

  // Log information to logger
  LOG(LOG_CRITICAL, "System aborted: %s [%s:%d]\n\r", msg, file, line);

  // Halt system
  for (;;) {
    __asm__("hlt;");
  }
}
