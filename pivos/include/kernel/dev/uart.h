#ifndef KERNEL_DEV_UART_H_
#define KERNEL_DEV_UART_H_

// this should be changed to something smarter
#ifndef KDEV_UART_COUNT
#define KDEV_UART_COUNT 10
#endif  // KDEV_UART_RING_SIZE

#include <stdint.h>

#include <kernel/dev.h>

int32_t dev_uart_init(int32_t uart_id);
struct kdev_io *dev_uart_get(int32_t uart_id);

#endif  // KERNEL_DEV_UART_H_