#include <assert.h>
#include <kernel/logger.h>
#include <kernel/pmm.h>
#include <stddef.h>
#include <string.h>

//
// TODO: Use a stack-based allocation for better performances
//

// Static functions prototypes
static inline uintptr_t pmm_get_frame_ndx(physaddr_t const addr);
static inline uintptr_t pmm_get_frame_mask(physaddr_t const addr);
static void pmm_mark_as_allocated(physaddr_t const addr);
static void pmm_mark_range_as_allocated(physaddr_t start, physaddr_t end);
static physaddr_t pmm_alloc_frame(size_t i, size_t final);

// PMM Datas
static uint8_t pmm_bitmap[FRAME_BITMAP_SIZE] = {0};
static size_t pmm_next_frame = 0;

static inline uintptr_t pmm_get_frame_ndx(physaddr_t const addr) {
  return (addr >> 12u) / 8u;
}

static inline uintptr_t pmm_get_frame_mask(physaddr_t const addr) {
  return 1 << ((addr >> 12) % 8u);
}

static void pmm_mark_as_allocated(physaddr_t const addr) {
  assert(IS_PAGE_ALIGNED(addr));
  pmm_bitmap[pmm_get_frame_ndx(addr)] |= pmm_get_frame_mask(addr);
}

static void pmm_mark_range_as_allocated(physaddr_t start, physaddr_t end) {
  assert(IS_PAGE_ALIGNED(start));
  assert(IS_PAGE_ALIGNED(end));

  while (start <= end) {
    pmm_mark_as_allocated(start);
    start += PAGE_SIZE;
  }
  pmm_next_frame = end;
}

void pmm_init(void) {
  extern void *KERNEL_END; // See linker's script

  pmm_next_frame = 0u;
  memset(pmm_bitmap, 0, sizeof(pmm_bitmap));
  pmm_mark_range_as_allocated(0, (((uintptr_t)&KERNEL_END) & ~PAGE_SIZE_MASK) +
                                     PAGE_SIZE);
  LOG(LOG_INFO, "PMM initialized\r\n");
}

static physaddr_t pmm_alloc_frame(size_t i, size_t final) {
  while (i < final) {
    if (pmm_bitmap[i] != 0xFFu) {
      size_t j = 0u;
      while (j < 7u && (pmm_bitmap[i] & (1 << j)) != 0) {
        ++j;
      }
      pmm_bitmap[i] |= (1 << j);
      pmm_next_frame = i;
      return (PAGE_SIZE * (i * 8u + j));
    }
    ++i;
  }
  return NULL_FRAME;
}

physaddr_t pmm_alloc(void) {
  physaddr_t frame = pmm_alloc_frame(pmm_next_frame, FRAME_BITMAP_SIZE);

  if (frame == NULL_FRAME) {
    frame = pmm_alloc_frame(0, pmm_next_frame);
  }
  return frame;
}

void pmm_free(physaddr_t const frame) {
  assert(IS_PAGE_ALIGNED(frame));
  assert(pmm_is_frame_allocated(frame));
  pmm_bitmap[pmm_get_frame_ndx(frame)] &= ~(pmm_get_frame_mask(frame));
  pmm_next_frame = pmm_get_frame_ndx(frame);
}

bool pmm_is_frame_allocated(physaddr_t const frame) {
  assert(IS_PAGE_ALIGNED(frame));
  return pmm_bitmap[pmm_get_frame_ndx(frame)] & (pmm_get_frame_mask(frame));
}
