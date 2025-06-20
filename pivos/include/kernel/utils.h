#ifndef KERNEL_UTILS_H_
#define KERNEL_UTILS_H_

#include <stddef.h>
#include <stdint.h>
#include <kernel/assert.h>

// irq_lock/unlock shouldn't be in utils

static inline void idle() {
    __asm__ volatile("wfe");
}

static inline void bytecpy(uint8_t *dst, uint8_t *src, uint32_t n) {
    for (int32_t i = 0; i < n; i++) {
        dst[i] = src[i];
    }
}

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define BIT(bit) ((1) << bit)

#define ALIGN_DOWN(addr, align) ((addr) & ~((align) - 1))
#define ALIGN_UP(addr, align) (((addr) + (align) - 1) & ~((align) - 1))

#define KB_IN_B (1024ULL)
#define MB_IN_B (KB_IN_B * 1024)
#define GB_IN_B (MB_IN_B * 1024)
#define TB_IN_B (GB_IN_B * 1024)

#define MMU_ASID_MASK 0xFFFF000000000000ULL
#define KERNEL_SPACE_MASK 0xFFFF000000000000ULL
#define PA_TO_KERNEL_VA(ptr) (typeof(ptr))((uint64_t)(ptr) | KERNEL_SPACE_MASK)

#define static_assert(condition, msg) _Static_assert(condition, msg)

void *memory_set(void *ptr, int value, uint64_t num);
void *memory_copy(void *destination, const void *source, uint64_t num);

#endif  // KERNEL_UTILS_H_