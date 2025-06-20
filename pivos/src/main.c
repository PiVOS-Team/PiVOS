#include <kernel/int.h>
#include <kernel/log.h>
#include <kernel/memory.h>
#include <kernel/utils.h>

#include <kernel/dev.h>
#include <kernel/dev/generic_timer.h>
#include <kernel/dev/gicv2.h>
#include <kernel/dev/uart.h>

int32_t setup_devices() {
    int32_t status = 0;

    uint64_t irq_key = irq_lock();

    if ((status = dev_uart_init(0)))
        return status;

    if ((status = dev_gicv2_init()))
        return status;

    if ((status = dev_generic_timer_init(4)))
        return status;

    if ((status = kdev_register(0, (struct kdev_generic *)dev_gicv2_get())) < 0)
        return status;

    if ((status = kdev_register(1, (struct kdev_generic *)dev_generic_timer_get())) < 0)
        return status;

    if ((status = kdev_register(2, (struct kdev_generic *)dev_uart_get(0))) < 0)
        return status;

    int_register(INT_TYPE_cur_el_spn_irq, kdev_gic_get_dispatch(0));

    struct dev_gicv2_params gicv2_timer_params = {.priority = 0, .cpu = 1, .trigger = 2};
    kdev_gic_register_irq(0, 30, 1, &gicv2_timer_params);
    kdev_gic_enable_irq(0, 30);

    struct dev_gicv2_params gicv2_uart_params = {.priority = 0, .cpu = 1, .trigger = 2};
    kdev_gic_register_irq(0, 153, 2, &gicv2_uart_params);
    kdev_gic_enable_irq(0, 153);

    irq_unlock(irq_key);

    return status;
}

void test_action() {
}

static char msg[] = "PiVOS\r\n";

void kernel_main() {
    memory_init(GB_IN_B * 4);

    int_init_dispatch_tab();
    setup_devices();

    kdev_timer_set_action(1, test_action);

    int pos = 0;

    while (1) {
        pos = pos + kdev_io_write(2, (uint8_t *)msg + pos, sizeof(msg) - pos);
        pos %= sizeof(msg);
    }

    return;
}
