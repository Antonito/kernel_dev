#include <arch/x86/utils.h>

uint32_t atomic_exchange(uint32_t volatile *ptr, uint32_t val) {
  __asm__ __volatile__("xchgl %[val], %[ptr];"
                       : [val] "=a"(val)
                       : "a"(val), [ptr] "m"(*ptr)
                       : "memory");
  return val;
}

void repinsw(uint16_t port, uint8_t *data, uint32_t size) {
  __asm__ __volatile__("rep insw"
                       : "+D"(data), "+c"(size)
                       : "d"(port)
                       : "memory");
}

void repoutsw(uint16_t port, uint8_t *data, uint32_t size) {
  __asm__ __volatile__("rep outsw" : "+S"(data), "+c"(size) : "d"(port));
}
