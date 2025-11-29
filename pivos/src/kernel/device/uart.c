#define DEV_PROTECTED_INTERFACE
#define UART_PROTECTED_INTERFACE

#include <kernel/device/uart.h>
#include <kernel/io_data.h>

int32_t protected_uart_init(struct dev_uart* ctx) {
    // Call base init
    int32_t res = protected_dev_init((struct dev*)ctx);

    ctx->data->rx = io_data_get_text_input_buffer();
    ctx->data->tx = io_data_get_text_output_buffer();

    return res;
}

void protected_uart_tx_empty(struct dev_uart* ctx) {
    struct kbuffer_ring* tx_ring = ctx->data->tx;
    uint8_t byte = 0;

    while(1) {
        if(kbuffer_ring_read(tx_ring, &byte) == 0) {
            // End of data in tx buffer
            
            ctx->api->stop_tx(ctx);
            ctx->data->tx_enabled = 0;
            break;
        }

        if(ctx->api->try_write_byte(ctx, byte) == 0) {
            // End of space in uart memory
            break;
        }
    }
}

void protected_uart_rx_ready(struct dev_uart* ctx) {
    struct kbuffer_ring* rx_ring = ctx->data->rx;
    uint8_t byte = 0;

    while(1) {
        if(ctx->api->try_read_byte(ctx, &byte) == 0) {
            // End of data in uart rx memory
            break;
        }

        if(kbuffer_ring_write(rx_ring, byte) == 0) {
            // RX buffer overlow
        }
    }
}

void uart_toggle_rx(struct dev_uart* ctx, uint8_t state) {
    if(ctx->data->rx_enabled == state) {
        return;
    }

    ctx->data->rx_enabled = state;
    if(state) {
        ctx->api->start_rx(ctx);
    } else {
        ctx->api->stop_rx(ctx);
    }
}

void uart_toggle_tx(struct dev_uart* ctx, uint8_t state) {
    if(ctx->data->tx_enabled == state) {
        return;
    }

    ctx->data->tx_enabled = state;
    if(state) {
        ctx->api->start_tx(ctx);
    } else {
        ctx->api->stop_tx(ctx);
    }
}

uint8_t uart_get_rx_state(struct dev_uart* ctx) {
    return ctx->data->rx_enabled;
}

uint8_t uart_get_tx_state(struct dev_uart* ctx) {
    return ctx->data->tx_enabled;
}