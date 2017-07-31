#include <stdint.h>
#include <string.h>

//
// Strings
//
size_t strlen(char const *str) {
  char const *s1 = str;

  while (*s1) {
    ++s1;
  }
  return (size_t)(s1 - str);
}

//
// Memory
//

void *memset(void *buff, int c, size_t n) {
  uint8_t *cur = buff;

  while (n) {
    *cur = (uint8_t)c;
    ++cur;
    --n;
  }
  return buff;
}

void *memcpy(void *restrict dst, void const *restrict src, size_t n) {
  uint8_t *d = dst;
  uint8_t const *s = src;

  while (n) {
    *d = *s;
    ++d;
    ++s;
    --n;
  }
  return dst;
}

int memcmp(void const *s1, void const *s2, size_t n) {
  uint8_t const *_s1 = s1;
  uint8_t const *_s2 = s2;

  while (n) {
    if (*_s1 != *_s2) {
      return *_s1 - *_s2;
    }
    ++_s1;
    ++_s2;
    --n;
  }
  return 0;
}
