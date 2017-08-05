#pragma once

#include <stdbool.h>
#include <stdint.h>

struct spinlock {
  uint32_t depth;
  uint32_t locked;
};

void init_spinlock(struct spinlock *const spin);
void lock_spinlock(struct spinlock *const spin);
void unlock_spinlock(struct spinlock *const spin);
bool is_locked_spinlock(struct spinlock *const spin);
