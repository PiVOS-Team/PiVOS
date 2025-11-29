#ifndef KERNEL_PROC_H_
#define KERNEL_PROC_H_

#include <stdint.h>
#include <kernel/mem.h>
#include <kernel/utils.h>

#define PROC_KERNEL_STACK_SIZE 16 * KB_IN_B

enum proc_state {
    PROC_STATE_FREE,
    PROC_STATE_NEW,
    PROC_STATE_READY,
    PROC_STATE_RUNNING,
    PROC_STATE_BLOCKED
};

struct proc {
    uint16_t PID;
    enum proc_state state;

    void* k_sp;
    struct mem_ctx mem_ctx;
    uint8_t kernel_stack[PROC_KERNEL_STACK_SIZE];

    struct proc* next;
};

void proc_create_new(struct proc* ctx, uint16_t pid);

void proc_destroy(struct proc* ctx);

void proc_init_ctx(struct proc* ctx, void* sp, void* pc);

void proc_switch(struct proc* curr, struct proc* next);

#endif
