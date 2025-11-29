#include <kernel/device/dev_event.h>

static dev_event_handler s_handlers[DEV_EVENT__N];

void dev_event_init() {

}

void dev_event_post(struct dev* ctx, enum dev_event event) {
    if(event >= DEV_EVENT__N || s_handlers[event] == 0) {
        return;
    }

    s_handlers[event](ctx);
}

void dev_event_register_handler(enum dev_event event, dev_event_handler callback) {
    if(event >= DEV_EVENT__N || s_handlers[event] != 0) {
        return;
    }

    s_handlers[event] = callback;
}