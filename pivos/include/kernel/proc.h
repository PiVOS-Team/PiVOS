#ifndef KERNEL_PROC_H_
#define KERNEL_PROC_H_

#include <stdint.h>

#define KPROC_TEXT_SIZE 16384
#define KPROC_STACK_SIZE 8192

struct kproc_suspend_state {
    uint64_t sp;
    uint64_t elr;
    uint64_t general_regs[32];
};

struct kproc {
    uint8_t __attribute__((aligned(16))) text[KPROC_TEXT_SIZE];
    uint8_t __attribute__((aligned(16))) stack[KPROC_STACK_SIZE];
    struct kproc_suspend_state suspend;
};

int32_t kproc_start(uint8_t *text, uint32_t size, int64_t *rs);

int32_t kproc_exec(uint8_t *text, uint32_t size);

int32_t kproc_exit();

#endif  // KERNEL_PROC_H_