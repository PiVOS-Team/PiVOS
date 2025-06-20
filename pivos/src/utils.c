#include <kernel/utils.h>

void *memory_set(void *ptr, int value, uint64_t num) {
    unsigned char *ptr_c = ptr;

    for (uint32_t i = 0; i < num; i++) {
        ptr_c[i] = value;
    }
    return ptr;
}

void *memory_copy(void *destination, const void *source, uint64_t num) {
    unsigned char *dst_c       = destination;
    const unsigned char *src_c = source;

    for (uint32_t i = 0; i < num; i++) {
        dst_c[i] = src_c[i];
    }
    return destination;
}