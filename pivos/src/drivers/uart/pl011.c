#define UART_PROTECTED_INTERFACE

#include <drivers/uart/pl011/impl.h>
#include <drivers/uart/pl011/reg.h>
#include <kernel/device/dev_event.h>
#include <kernel/device/driver.h>
#include <kernel/device/irq_controller.h>

static inline struct uart* pl011_get_regs(struct dev_uart* ctx) {
    return (struct uart*)ctx->config->mem->base;
}

static enum irq_handle_status pl011_irq_handler(struct dev* ctx) {
    struct dev_uart* uart_ctx = (struct dev_uart*)ctx;
    struct uart* reg = pl011_get_regs(uart_ctx);
    union uart_mis mis = {.bits = reg->MIS.bits};

    if (mis.fields.TXMIS) {
        protected_uart_tx_empty(uart_ctx);
    }

    if (mis.fields.RXMIS) {
        protected_uart_rx_ready(uart_ctx);
    }

    return IRQ_HANDLED;
}

static int32_t api_pl011_init(struct dev* ctx) {
    struct dev_uart* uart_ctx = (struct dev_uart*)ctx;
    struct dev_pl011* pl011_ctx = (struct dev_pl011*)ctx;

    // Call base init
    protected_uart_init(uart_ctx);

    // Ensure irq controller is initialized
    struct dev_irq_controller* irq_ctrl = ctx->config->irq_controller;
    dev_event_post((struct dev*)irq_ctrl, DEV_EVENT_ENSURE_DEVICE_INITIALIZED);

    uint16_t irq_number = pl011_ctx->config->irq_number;
    struct irq_params params = {
        .target_cpu = IRQ_DEFAULT_CPU,
        .priority = IRQ_PRIORITY_REGULAR,
        .trigger_type = IRQ_TRIGGER_EDGE};

    struct uart* reg = pl011_get_regs(uart_ctx);

    // disable uart
    reg->CR.bits = 0;

    // baudrate
    reg->IBRD.bits = 26;
    reg->FBRD.bits = 0;

    // datasize
    union uart_lcrh lcrh = {.bits = 0x0};
    lcrh.fields.WLEN = 0b11;
    lcrh.fields.FEN = 0x1;
    reg->LCRH.bits = lcrh.bits;

    // interrupt masks
    union uart_imsc imsc = {.bits = 0x0};
    imsc.fields.TXIM = 0;
    imsc.fields.RXIM = 0;
    reg->IMSC.bits = imsc.bits;

    union uart_ifls ifls = {.bits = reg->IFLS.bits};
    ifls.fields.RXIFLSEL = 0b010;
    ifls.fields.TXIFLSEL = 0b010;
    reg->IFLS.bits = ifls.bits;

    // enable uart
    union uart_cr cr = {.bits = 0x0};
    cr.fields.UARTEN = 1;
    cr.fields.TXE = 1;
    cr.fields.RXE = 1;

    // hardware control flow
    cr.fields.CTSEN = 0;
    cr.fields.RTSEN = 0;
    reg->CR.bits = cr.bits;

    // PL011 uart has only one irq
    irq_controller_register(irq_ctrl, irq_number, &params, pl011_irq_handler, ctx);
    irq_controller_toggle(irq_ctrl, irq_number, 1);

    return 1;
}

static int32_t api_pl011_destroy(struct dev* ctx) {
    return 0;
}

static void api_pl011_start_rx(struct dev_uart* ctx) {
    struct uart* reg = pl011_get_regs(ctx);
    union uart_imsc imsc = reg->IMSC;
    imsc.fields.RXIM = 1;
    reg->IMSC.bits = imsc.bits;
}

static void api_pl011_stop_rx(struct dev_uart* ctx) {
    struct uart* reg = pl011_get_regs(ctx);
    union uart_imsc imsc = reg->IMSC;
    imsc.fields.RXIM = 0;
    reg->IMSC.bits = imsc.bits;
}

static void api_pl011_start_tx(struct dev_uart* ctx) {
    struct uart* reg = pl011_get_regs(ctx);
    union uart_imsc imsc = reg->IMSC;
    imsc.fields.TXIM = 1;
    reg->IMSC.bits = imsc.bits;
    protected_uart_tx_empty(ctx);
}

static void api_pl011_stop_tx(struct dev_uart* ctx) {
    struct uart* reg = pl011_get_regs(ctx);
    union uart_imsc imsc = reg->IMSC;
    imsc.fields.TXIM = 0;
    reg->IMSC.bits = imsc.bits;
}

static uint8_t api_pl011_try_read_byte(struct dev_uart* ctx, uint8_t* byte) {
    struct uart* reg = pl011_get_regs(ctx);

    // Check if there are any bytes left in FIFO
    if (reg->FR.fields.RXFE) {
        return 0;
    }

    *byte = reg->DR.fields.DATA;

    return 1;
}

static uint8_t api_pl011_try_write_byte(struct dev_uart* ctx, uint8_t byte) {
    struct uart* reg = pl011_get_regs(ctx);

    if (reg->FR.fields.TXFF) {
        return 0;
    }

    // Check if there are any bytes left in buffer to send
    union uart_dr dr;
    dr.fields.DATA = byte;
    reg->DR.bits = dr.bits;

    return 1;
}

static struct dev_pl011_api pl011_api = {
    .init = api_pl011_init,
    .destroy = api_pl011_destroy,
    .start_rx = api_pl011_start_rx,
    .stop_rx = api_pl011_stop_rx,
    .start_tx = api_pl011_start_tx,
    .stop_tx = api_pl011_stop_tx,
    .try_read_byte = api_pl011_try_read_byte,
    .try_write_byte = api_pl011_try_write_byte,
};

static void drv_pl011_update_ctx(struct dev* ctx) {
    struct dev_pl011* uart_pl011 = (struct dev_pl011*)ctx;
    uart_pl011->api = &pl011_api;
}

DRV_REGISTER_NEW(pl011, BUS_TYPE_MMIO, DEV_TYPE_UART, drv_pl011_update_ctx)
