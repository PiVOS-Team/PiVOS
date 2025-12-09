#ifndef DRIVERS_UART_PL011_IMPL_H_
#define DRIVERS_UART_PL011_IMPL_H_

#include <kernel/device/uart.h>

#define DEV_pl011_CONFIG_FIELDS \
    DEV_uart_CONFIG_FIELDS      \
        uint16_t irq_number;

#define DEV_pl011_API_FIELDS \
    DEV_uart_API_FIELDS

#define DEV_pl011_DATA_FIELDS \
    DEV_uart_DATA_FIELDS

DEV_DEFINE_NEW(pl011)

#endif