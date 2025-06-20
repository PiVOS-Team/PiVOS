#include <kernel/assert.h>

#ifdef DEBUG

extern char stack_bottom;
extern char stack_top;

void assert_check(uint8_t condition_result, char *message) {
    if (condition_result) {
        return;
    }

    uint32_t i = 0;
    uint64_t max_length = &stack_top - &stack_bottom;
    uint64_t base_addr = (uint64_t)&stack_bottom;
    
    while (message[i] != '\0' && i != max_length) {
        *(uint8_t*)(base_addr + i + sizeof(uint32_t)) = message[i];
        i++;
    }

    *(uint32_t*)base_addr = i;

    while (1) {
    }
}

#endif