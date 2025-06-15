#ifndef KERNEL_PROC_H_
#define KERNEL_PROC_H_

#include <stdint.h>

#include <kernel/memory.h>

#ifndef KPROC_MAX_COUNT
#define KPROC_MAX_COUNT 16
#endif

#ifndef KPROC_PROG_OFFSET
#define KPROC_PROG_OFFSET 0x80000ULL
#endif

struct kproc_suspend_state {
    uint64_t sp;
    uint64_t elr;
    uint64_t general_regs[32];
};

struct kproc {
    struct memory_context ctx;
    struct kproc_suspend_state suspend;
};

struct kproc_prog_entry {
    void *text;
    uint64_t size;
};

void kproc_exec(uint32_t prog_id);

int32_t kproc_start(uint32_t prog_id, int64_t *rs);

int32_t kproc_exit();

#endif  // KERNEL_PROC_H_
