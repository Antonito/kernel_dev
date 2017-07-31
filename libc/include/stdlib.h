#pragma once

#if defined__cplusplus
extern "C" {
#endif

#define abort() abort_handler("System aborted", __FILE__, __LINE__)

// Should not be called alone, use abort()
__attribute__((noreturn)) void abort_handler();

#if defined__cplusplus
}
#endif
