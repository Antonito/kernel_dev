#pragma once

#include <stdlib.h>

#if defined NDEBUG
#define assert(x) (void)0
#else
#define assert(x)                                                              \
  do {                                                                         \
    if (!(x)) {                                                                \
      abort_handler(#x, __FILE__, __LINE__);                                   \
    }                                                                          \
  } while (0)
#endif

// TODO: Debug assert()
