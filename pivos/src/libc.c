#include <stddef.h>
#include <stdint.h>

void *memset(void *ptr, int value, size_t num) {
    unsigned char *ptr_c = ptr;

    for (uint32_t i = 0; i < num; i++) {
        ptr_c[i] = value;
    }
    return ptr;
}

void *memcpy(void *destination, const void *source, size_t num) {
    unsigned char *dst_c = destination;
    const unsigned char *src_c = source;

    for (uint32_t i = 0; i < num; i++) {
        dst_c[i] = src_c[i];
    }
    return destination;
}