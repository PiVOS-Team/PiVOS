#define DEV_PROTECTED_INTERFACE

#include <kernel/device/dev.h>

int32_t protected_dev_init(struct dev* ctx) {
    return 1;
}

int32_t dev_init(struct dev* ctx) {
    int32_t result = ctx->api->init(ctx);
    ctx->status = DEV_STATUS_INITIALIZED;

    return result;
}