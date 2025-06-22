#ifndef KERNEL_BUFFER_STACK_H_
#define KERNEL_BUFFER_STACK_H_

#include <kernel/utils.h>
#include <stdint.h>

struct kbuffer_stack {
    uint8_t *buffer;
    uint32_t capacity;
    uint32_t len;
    uint32_t align;
};

#define KBUFFER_STACK_INIT(_capacity, _buffer, _align)          \
    (struct kbuffer_stack) {                                    \
        .buffer = (_buffer), .capacity = (_capacity), .len = 0, \
        .align = (_align)                                       \
    }

static inline uint64_t kbuffer_stack_full(struct kbuffer_stack *stack) {
    return stack->len == stack->capacity;
}

static inline uint64_t kbuffer_stack_size(struct kbuffer_stack *stack) {
    return stack->len;
}

static inline uint8_t *kbuffer_stack_peek(struct kbuffer_stack *stack) {
    if (stack->len == 0) {
        return 0;
    }

    return stack->buffer + (stack->len - 1) * stack->align;
}

static inline uint8_t *kbuffer_stack_alloc(struct kbuffer_stack *stack) {
    if (kbuffer_stack_full(stack)) {
        return 0;
    }

    stack->len++;
    uint8_t *addr = kbuffer_stack_peek(stack);

    return addr;
}

static inline void kbuffer_stack_free(struct kbuffer_stack *stack) {
    if (stack->len > 0) {
        stack->len--;
    }
}

#endif  // KERNEL_BUFFER_STACK_H_
