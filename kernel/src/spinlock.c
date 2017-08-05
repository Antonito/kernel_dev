#include <arch/x86/utils.h>
#include <assert.h>
#include <kernel/spinlock.h>

void init_spinlock(struct spinlock *const spin) {
  spin->depth = 0;
  spin->locked = 0;
}

void lock_spinlock(struct spinlock *const spin) {
  ++spin->depth;
  while (!atomic_exchange(&spin->locked, 1))
    ;
}

void unlock_spinlock(struct spinlock *const spin) {
  assert(is_locked_spinlock(spin) && spin->depth > 0);
  --spin->depth;
  if (!spin->depth) {
    // No one is holding the lock anymore
    atomic_exchange(&spin->locked, 0);
  }
}

bool is_locked_spinlock(struct spinlock *const spin) {
#ifndef NDEBUG
  if (spin->locked) {
    assert(spin->depth > 0);
  }
#endif
  return spin->locked;
}
