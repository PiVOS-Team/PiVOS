#include <kernel/syscall.h>

static syscall_handler s_handlers[SYSCALL__N];

int32_t syscall_init() {
    return 1;
}

void syscall_dispatch(uint16_t syscall_number, int64_t* args) {
    if(syscall_number >= SYSCALL__N || s_handlers[syscall_number] == 0) {
        return;
    }

    s_handlers[syscall_number](args);
}

void syscall_register_handler(enum syscall_type syscall_number, syscall_handler handler) {
    if(syscall_number >= SYSCALL__N || s_handlers[syscall_number] != 0) {
        return;
    }

    s_handlers[syscall_number] = handler;
}