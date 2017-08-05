#pragma once

#include <stdbool.h>
#include <stdint.h>

// Physical addr
typedef uintptr_t physaddr_t;

// Useful definitions
#define PAGE_SIZE (4096u)
#define PAGE_SIZE_MASK ((PAGE_SIZE)-1u)
#define NB_FRAMES ((UINTPTR_MAX / PAGE_SIZE) + 1)
#define FRAME_BITMAP_SIZE (NB_FRAMES / 8u)
#define IS_PAGE_ALIGNED(x) (!((uintptr_t)(x)&PAGE_SIZE_MASK))
#define NULL_FRAME (-1u)

void pmm_init(void);

physaddr_t pmm_alloc(void);
void pmm_free(physaddr_t const frame);
bool pmm_is_frame_allocated(physaddr_t const frame);
