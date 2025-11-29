#ifndef KERNEL_USERSPACE_H_
#define KERNEL_USERSPACE_H_

#include <stdint.h>
#include <kernel/device.h>

int32_t userspace_init(struct dev_uart* console, struct dev_timer* sys_timer);

void userspace_start();

#endif
