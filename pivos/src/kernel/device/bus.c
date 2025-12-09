#define DEV_PROTECTED_INTERFACE
#define BUS_PROTECTED_INTERFACE

#include <kernel/device/bus.h>

int32_t protected_bus_init(struct dev_bus* ctx) {
    // Call base init
    return protected_dev_init((struct dev*)ctx);
}

void bus_search(struct dev_bus* ctx) {
    ctx->api->search(ctx);
}