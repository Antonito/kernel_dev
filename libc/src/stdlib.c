#include <private/kernel.h>
#include <stdlib.h>

// Prototype
__attribute__((noreturn)) void abort_handler(const char *const msg,
                                             const char *const file,
                                             const char *const line);

// Abort the system. Shall never return
__attribute__((noreturn)) void abort_handler(const char *const msg,
                                             const char *const file,
                                             const char *const line) {
  // Disable interrupts
  __asm__("cli;");

  // Log information to logger
  g_klogger.set_level(LOG_CRITICAL);
  LOG("System aborted: %s [%s:%s]\n", msg, file, line);

  // Halt system
  for (;;) {
    __asm__("hlt;");
  }
}
