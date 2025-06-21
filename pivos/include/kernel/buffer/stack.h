#ifndef KERNEL_BUFFER_STACK_H_
#define KERNEL_BUFFER_STACK_H_

#include <stdint.h>

#include <kernel/utils.h>

struct kbuffer_stack {
    uint8_t *buffer;
    uint32_t capacity;
    uint32_t idx;
    uint32_t align;
};

#define KBUFFER_STACK_INIT(_capacity, _buffer, _align)                            \
    (struct kbuffer_stack) {                                                      \
        .buffer = (_buffer), .capacity = (_capacity), .idx = 0, .align = (_align) \
    }

static inline uint8_t *kbuffer_stack_peek(struct kbuffer_stack *stack) {
    if (stack->idx == 0) {
        return 0;
    }

    return stack->buffer + (stack->idx - 1) * stack->align;
}

static inline uint8_t *kbuffer_stack_alloc(struct kbuffer_stack *stack) {
    if (stack->idx + 1 >= stack->capacity) {
        return 0;
    }

    stack->idx++;
    uint8_t *addr = kbuffer_stack_peek(stack);


    return addr;
}

static inline void kbuffer_stack_free(struct kbuffer_stack *stack) {
    if (stack->idx > 0) {
        stack->idx--;
    }
}

#endif  // KERNEL_BUFFER_STACK_H_