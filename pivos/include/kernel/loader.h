#ifndef KERNEL_LOADER_H_
#define KERNEL_LOADER_H_

#include <kernel/proc.h>

int32_t loader_prepare_proc(struct proc* new_proc, int image_id);

#endif
