#include <kernel/dev.h>

#include <kernel/dev/generic_timer.h>
#include <kernel/dev/generic_timer/impl.h>
#include <kernel/dev/generic_timer/reg.h>

static uint16_t interval = 0;
static void (*timer_action)();
static struct kdev_timer kdev;

int32_t dev_generic_timer_init(uint16_t _interval) {
    interval = _interval;
    generic_timer_reset();
    kdev = (struct kdev_timer){
        .type       = KDEV_TIMER,
        .ctx        = 0,
        .isr        = dev_generic_timer_kdev_isr,
        .set_action = dev_generic_timer_kdev_set_action,
    };
    return 0;
}

struct kdev_timer *dev_generic_timer_get() {
    return &kdev;
}

void dev_generic_timer_kdev_isr(void *ctx) {
    timer_action();
    generic_timer_reset();
}

void dev_generic_timer_kdev_set_action(void *ctx, void (*action)(void)) {
    timer_action = action;
}

void generic_timer_reset() {
    union cntp_tval_el0_t tval;
    union cntfrq_el0_t frq;
    union cntp_ctl_el0_t ctl;

    asm volatile("mrs %[dst], cntfrq_el0" : [dst] "=r"(frq));

    uint32_t freq          = frq.fields.ClockFrequency;
    tval.fields.TimerValue = freq * interval;

    ctl.fields.ISTATUS = 0;
    ctl.fields.ENABLE  = 1;

    asm volatile("msr cntp_tval_el0, %[src]" : : [src] "r"(tval.bits));
    asm volatile("msr cntp_ctl_el0, %[src]" : : [src] "r"(ctl.bits));
}
