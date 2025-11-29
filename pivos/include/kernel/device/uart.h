#ifndef KERNEL_DEVICE_UART_H_
#define KERNEL_DEVICE_UART_H_

#include <kernel/device/dev.h>
#include <kernel/buffer/ring.h>

#define DEV_uart_CONFIG_FIELDS  \
    DEV_dev_CONFIG_FIELDS

#define DEV_uart_API_FIELDS                                         \
    DEV_dev_API_FIELDS                                              \
    void (*start_tx)(struct dev_uart* ctx);                         \
    void (*stop_tx)(struct dev_uart* ctx);                          \
    void (*start_rx)(struct dev_uart* ctx);                         \
    void (*stop_rx)(struct dev_uart* ctx);                          \
    uint8_t (*try_write_byte)(struct dev_uart* ctx, uint8_t byte);  \
    uint8_t (*try_read_byte)(struct dev_uart* ctx, uint8_t* byte);

#define DEV_uart_DATA_FIELDS    \
    DEV_dev_DATA_FIELDS         \
    volatile uint8_t rx_enabled;\
    volatile uint8_t tx_enabled;\
    struct kbuffer_ring* tx;    \
    struct kbuffer_ring* rx;

DEV_DEFINE_NEW(uart)

#ifdef UART_PROTECTED_INTERFACE

int32_t protected_uart_init(struct dev_uart* ctx);

void protected_uart_tx_empty(struct dev_uart* ctx);

void protected_uart_rx_ready(struct dev_uart* ctx);

#endif

void uart_toggle_rx(struct dev_uart* ctx, uint8_t state);

void uart_toggle_tx(struct dev_uart* ctx, uint8_t state);

uint8_t uart_get_rx_state(struct dev_uart* ctx);

uint8_t uart_get_tx_state(struct dev_uart* ctx);

#endif