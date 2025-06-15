#ifndef KERNEL_CALL_DEV_H_
#define KERNEL_CALL_DEV_H_

#include <kernel/call.h>

void kcall_io_read(int64_t *rs);

void kcall_io_write(int64_t *rs);

void kcall_io_poll_out(int64_t *rs);

#endif  // KERNEL_CALL_DEV_H_