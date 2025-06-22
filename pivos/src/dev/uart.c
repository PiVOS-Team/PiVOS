#include <kernel/buffer/ring.h>
#include <kernel/dev.h>
#include <kernel/dev/uart.h>
#include <kernel/dev/uart/impl.h>
#include <kernel/dev/uart/reg.h>
#include <kernel/int.h>

static struct dev_uart uart_impls[KDEV_UART_COUNT];
static struct kdev_io uart_kdevs[KDEV_UART_COUNT];

int32_t dev_uart_kdev_write(void *ctx, uint8_t *src, uint32_t n) {
    struct dev_uart *dev = ctx;
    int32_t idx = 0;
    uint64_t irq_key = irq_lock();
    for (; idx < n; idx++) {
        if (kbuffer_ring_write(&dev->tx, src[idx]) < 0) {
            break;
        }

        dev_uart_sync_write(ctx);
    }
    irq_unlock(irq_key);
    return idx;
}

int32_t dev_uart_kdev_read(void *ctx, uint8_t *dst, uint32_t n) {
    struct dev_uart *dev = ctx;

    int32_t idx = 0;
    int32_t res = 0;

    uint64_t irq_key = irq_lock();
    for (; idx < n; idx++) {
        dev_uart_sync_read(ctx);

        if ((res = kbuffer_ring_read(&dev->rx)) < 0) {
            break;
        }
        dst[idx] = res;
    }
    irq_unlock(irq_key);
    return idx;
}

int32_t dev_uart_sync_write(void *ctx) {
    struct dev_uart *dev = ctx;

    while (1) {
        union uart_fr fr = {.bits = dev->reg->FR.bits};
        if (fr.fields.TXFF) {
            return -1;
        }

        int32_t res = kbuffer_ring_read(&dev->tx);
        if (res < 0) {
            return 0;
        }

        union uart_dr dr;
        dr.fields.DATA = (uint8_t)res;
        dev->reg->DR.bits = dr.bits;
    }
}

int32_t dev_uart_sync_read(void *ctx) {
    struct dev_uart *dev = ctx;

    while (1) {
        union uart_fr fr = {.bits = dev->reg->FR.bits};
        if (fr.fields.RXFE) {
            return 0;
        }

        if (!kbuffer_ring_available(&dev->rx)) {
            return -1;
        }

        union uart_dr dr = {.bits = dev->reg->DR.bits};
        kbuffer_ring_write(&dev->rx, dr.fields.DATA);
    }
}

int32_t dev_uart_kdev_isr(void *ctx) {
    struct dev_uart *dev = ctx;
    union uart_mis mis = {.bits = dev->reg->MIS.bits};

    if (mis.fields.TXMIS) {
        dev_uart_sync_write(ctx);
    }

    if (mis.fields.RXMIS) {
        dev_uart_sync_read(ctx);
    }
    return KSTATUS_DEV_IO_OK;
}

int32_t dev_uart_kdev_pool_out(void *ctx, uint8_t c) {
    struct dev_uart *dev = ctx;

    union uart_fr fr = {.bits = dev->reg->FR.bits};
    if (fr.fields.TXFF) {
        return KSTATUS_DEV_IO_TX_WAIT;
    }
    union uart_dr dr;
    dr.fields.DATA = c;
    dev->reg->DR.bits = dr.bits;
    return KSTATUS_DEV_IO_OK;
}

int32_t dev_uart_kdev_pool_in(void *ctx, uint8_t *c) {
    struct dev_uart *dev = ctx;

    union uart_fr fr = {.bits = dev->reg->FR.bits};
    if (fr.fields.RXFE) {
        return KSTATUS_DEV_IO_RX_WAIT;
    }

    union uart_dr dr = {.bits = dev->reg->DR.bits};
    *c = dr.fields.DATA;
    return KSTATUS_DEV_IO_OK;
}

int32_t dev_uart_init(int32_t uart_id) {
    struct uart *reg = (struct uart *)((UART_ADDRESS) + 0x200 * uart_id);
    uart_impls[uart_id] = (struct dev_uart){
        .reg = reg,
        .tx =
            KBUFFER_RING_INIT(KDEV_UART_RING_SIZE, uart_impls[uart_id].tx_buf),
        .rx =
            KBUFFER_RING_INIT(KDEV_UART_RING_SIZE, uart_impls[uart_id].rx_buf),
    };

    uart_kdevs[uart_id] = (struct kdev_io){
        .type = KDEV_IO,
        .ctx = &uart_impls[uart_id],
        .isr = dev_uart_kdev_isr,
        .write = dev_uart_kdev_write,
        .read = dev_uart_kdev_read,
        .pool_in = dev_uart_kdev_pool_in,
        .pool_out = dev_uart_kdev_pool_out,
    };

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
    imsc.fields.TXIM = 1;
    imsc.fields.RXIM = 1;
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

    return 0;
}

struct kdev_io *dev_uart_get(int32_t uart_id) {
    return &uart_kdevs[uart_id];
}
