#ifndef KERNEL_PROC_H_
#define KERNEL_PROC_H_

#include <stdint.h>

struct kproc {
    uint64_t *text;
    uint64_t *stack;
};

int32_t kproc_exec(struct kproc *proc);

int32_t kproc_exit();

#endif  // KERNEL_PROC_H_