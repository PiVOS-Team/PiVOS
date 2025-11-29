#ifndef KERNEL_SYSCALL_H_
#define KERNEL_SYSCALL_H_

#include <stdint.h>

enum syscall_type {
    SYSCALL_PROC_SPAWN,
    SYSCALL_PROC_EXIT,

    SYSCALL_WRITE,
    SYSCALL_YIELD,

    SYSCALL__N
};

typedef void (*syscall_handler)(int64_t* args);

int32_t syscall_init();

void syscall_dispatch(uint16_t syscall_number, int64_t* args);

void syscall_register_handler(enum syscall_type syscall_number, syscall_handler handler);

#endif
