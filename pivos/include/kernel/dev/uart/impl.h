#ifndef KERNEL_DEV_UART_IMPL_H_
#define KERNEL_DEV_UART_IMPL_H_

#include <kernel/buffer/ring.h>
#include <kernel/dev/uart/reg.h>

#ifndef KDEV_UART_RING_SIZE
#define KDEV_UART_RING_SIZE 4096
#endif  // KDEV_UART_RING_SIZE

struct dev_uart {
    struct uart *reg;
    struct kbuffer_ring tx, rx;
    uint8_t tx_buf[KDEV_UART_RING_SIZE];
    uint8_t rx_buf[KDEV_UART_RING_SIZE];
};

int32_t dev_uart_sync_write(void *ctx);
int32_t dev_uart_sync_read(void *ctx);

#endif  // KERNEL_DEV_UART_IMPL_H_