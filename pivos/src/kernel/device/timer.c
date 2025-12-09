#define DEV_PROTECTED_INTERFACE
#define TIMER_PROTECTED_INTERFACE

#include <kernel/device/dev_event.h>
#include <kernel/device/timer.h>

int32_t protected_timer_init(struct dev_timer* ctx) {
    // Call base init
    int32_t res = protected_dev_init((struct dev*)ctx);

    ctx->data->interval = 0;
    ctx->data->total_time = 0;
    ctx->data->is_running = 0;

    return res;
}

void protected_timer_tick(struct dev_timer* ctx) {
    ctx->data->total_time += ctx->data->interval;

    dev_event_post((struct dev*)ctx, DEV_EVENT_TIMER_TICK);
}

void timer_start(struct dev_timer* ctx) {
    if (ctx->data->is_running == 1) {
        return;
    }

    ctx->api->start(ctx);
}

void timer_stop(struct dev_timer* ctx) {
    if (ctx->data->is_running == 0) {
        return;
    }

    ctx->api->stop(ctx);
}

void timer_set_interval(struct dev_timer* ctx, uint16_t count) {
    ctx->data->interval = count;

    if (ctx->data->is_running) {
        ctx->api->stop(ctx);
        ctx->api->update_interval(ctx);
        ctx->api->start(ctx);
    } else {
        ctx->api->update_interval(ctx);
    }
}

uint16_t timer_get_interval(struct dev_timer* ctx) {
    return ctx->data->interval;
}

uint16_t timer_get_total_time(struct dev_timer* ctx) {
    return ctx->data->total_time;
}