#ifndef KERNEL_CALL_H_
#define KERNEL_CALL_H_

#include <stdint.h>

#ifndef KCALL_BUFFER_SIZE
#define KCALL_BUFFER_SIZE 1024
#endif  // KCALL_BUFFER_SIZE

typedef void (*kcall_fn)(int64_t *);

int32_t kcall_register(uint16_t call_id, kcall_fn call);

void kcall_dispatch(int64_t *rs, uint64_t esr);

#endif  // KERNEL_CALL_H_