#ifndef KERNEL_BUFFER_RING_H_
#define KERNEL_BUFFER_RING_H_

#include <kernel/utils.h>
#include <stdint.h>

struct kbuffer_ring {
    uint8_t *buffer;
    uint32_t capacity;
    uint32_t size;
    uint32_t read_idx, write_idx;
};

#define KBUFFER_RING_INIT(_capacity, _buffer)                    \
    (struct kbuffer_ring) {                                      \
        .buffer = (_buffer), .capacity = (_capacity), .size = 0, \
        .read_idx = 0, .write_idx = 0,                           \
    }

static inline int32_t kbuffer_ring_write(struct kbuffer_ring *buf, uint8_t c) {
    if (buf->size == buf->capacity) {
        return -1;
    }

    buf->buffer[buf->write_idx] = c;
    buf->write_idx = (buf->write_idx + 1) % buf->capacity;
    buf->size++;
    return 0;
}

static inline int32_t kbuffer_ring_read(struct kbuffer_ring *buf) {
    if (buf->size == 0) {
        return -1;
    }

    int32_t c = buf->buffer[buf->read_idx];
    buf->read_idx = (buf->read_idx + 1) % buf->capacity;
    buf->size--;
    return c;
}

static inline int32_t kbuffer_ring_free(struct kbuffer_ring *buf, uint32_t n) {
    uint32_t to_free = (n > buf->size) ? buf->size : n;
    buf->read_idx = (buf->read_idx + to_free) % buf->capacity;
    buf->size -= to_free;
    return (int32_t)to_free;
}

static inline uint32_t kbuffer_ring_available(const struct kbuffer_ring *buf) {
    return buf->capacity - buf->size;
}

static inline uint32_t kbuffer_ring_size(const struct kbuffer_ring *buf) {
    return buf->size;
}

static inline int32_t kbuffer_ring_reset(struct kbuffer_ring *buf) {
    buf->size = 0;
    buf->read_idx = 0;
    buf->write_idx = 0;
    return 0;
}

#endif  // KERNEL_BUFFER_RING_H_