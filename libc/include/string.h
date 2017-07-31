#pragma once

#include <stddef.h>

#if defined__cplusplus
extern "C" {
#endif

// String utilities
size_t strlen(char const *str);

// Memory utilities
void *memset(void *buff, int c, size_t n);
void *memcpy(void *restrict dst, void const *restrict src, size_t n);
int memcmp(void const *s1, void const *s2, size_t n);

#if defined__cplusplus
}
#endif
