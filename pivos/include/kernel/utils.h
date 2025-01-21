#ifndef KERNEL_UTILS_H_
#define KERNEL_UTILS_H_

#include <stdint.h>

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

#define static_assert(condition, msg) _Static_assert(condition, msg)
#define offsetof(type, member) __builtin_offsetof(type, member)

#endif  // KERNEL_UTILS_H_