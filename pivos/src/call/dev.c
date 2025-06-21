#include <kernel/call/dev.h>

#include <kernel/dev.h>

void kcall_io_read(int64_t *rs) {
    int64_t status = kdev_io_read(*((uint32_t *)(rs + 0)),
        *((uint8_t **)(rs + 1)), *((uint32_t *)(rs + 2)));

    rs[0] = status;
}

void kcall_io_write(int64_t *rs) {
    int64_t status = kdev_io_write(*((uint32_t *)(rs + 0)),
        *((uint8_t **)(rs + 1)), *((uint32_t *)(rs + 2)));

    rs[0] = status;
}

void kcall_io_poll_out(int64_t *rs) {
    int64_t status = kdev_io_pool_out(*((uint32_t *)(rs + 0)), *((uint8_t *)(rs + 1)));

    rs[0] = status;
}