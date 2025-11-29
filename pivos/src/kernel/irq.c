#include <kernel/irq.h>

static struct dev_irq_controller* s_main_controller;
static irq_post_handler s_post_handler;

void irq_init(struct dev_irq_controller* main_controller) {
    s_main_controller = main_controller;
}

void irq_register_post_handler(irq_post_handler callback) {
    s_post_handler = callback;
}

void irq_dispatch() {
    irq_controller_dispatch(s_main_controller);

    if(s_post_handler) {
        s_post_handler();
    }
}