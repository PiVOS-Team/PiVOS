#include <kernel/call/proc.h>

#include <kernel/proc.h>

void kcall_proc_start(int64_t *rs) {
    int64_t status = kproc_start(*((uint8_t **)(rs + 0)), *((uint32_t *)(rs + 1)), rs);

    rs[0] = status;
}

void kcall_proc_exit(int64_t *rs) {
    int64_t status = kproc_exit();

    rs[0] = status;
}