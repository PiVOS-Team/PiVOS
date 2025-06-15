#ifndef KERNEL_CALL_PROC_H_
#define KERNEL_CALL_PROC_H_

#include <kernel/call.h>

void kcall_proc_start(int64_t *rs);

void kcall_proc_exit(int64_t *rs);

#endif  // KERNEL_CALL_PROC_H_