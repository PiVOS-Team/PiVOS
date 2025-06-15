#include <kernel/call.h>
#include <kernel/call/dev.h>
#include <kernel/int.h>
#include <kernel/log.h>
#include <kernel/proc.h>
#include <kernel/utils.h>

#include <kernel/dev.h>
#include <kernel/dev/generic_timer.h>
#include <kernel/dev/gicv2.h>
#include <kernel/dev/uart.h>

#include <kernel/test.h>

int32_t setup_devices() {
    int32_t status = 0;

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
    int_register(INT_TYPE_low_el_aarch64_irq, kdev_gic_get_dispatch(0));

    struct dev_gicv2_params gicv2_timer_params = {.priority = 0, .cpu = 1, .trigger = 2};
    kdev_gic_register_irq(0, 30, 1, &gicv2_timer_params);
    kdev_gic_enable_irq(0, 30);

    struct dev_gicv2_params gicv2_uart_params = {.priority = 0, .cpu = 1, .trigger = 2};
    kdev_gic_register_irq(0, 153, 2, &gicv2_uart_params);
    kdev_gic_enable_irq(0, 153);

    return status;
}

void kcall_test(int64_t *rs) {
    rs[0] *= 2;
}

int32_t setup_syscalls() {
    int32_t status = 0;

    int_register(INT_TYPE_low_el_aarch64_sync, kcall_dispatch);

    if ((status = kcall_register(1, kcall_test)) < 0)
        return status;

    if ((status = kcall_register(2, kcall_io_write)) < 0)
        return status;

    if ((status = kcall_register(3, kcall_io_read)) < 0)
        return status;

    if ((status = kcall_register(4, kcall_io_poll_out)) < 0)
        return status;


    return status;
}

void test_action() {
}

static char msg[] = "PiVOS\r\n";

static uint64_t el0_stack[4096];

void kernel_main() {
    uint64_t irq_key = irq_lock();

    int_init_dispatch_tab();
    setup_devices();
    setup_syscalls();

    kdev_timer_set_action(1, test_action);

    irq_unlock(irq_key);

    bytecpy((uint8_t *)0x10000000UL, a_img, a_img_len);

    struct kproc proc = {.stack = el0_stack, .text = (uint64_t *)0x10000000UL};

    kproc_exec(&proc);

    return;
}
