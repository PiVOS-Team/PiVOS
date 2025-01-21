#ifndef KERNEL_INT_H_
#define KERNEL_INT_H_

#ifdef __ASSEMBLER__

#endif  // __ASSEMBLER__

#ifndef __ASSEMBLER__

#include <stdint.h>

typedef void (*int_dispatch_fn)(int64_t *);

extern void vector_table();

#define INT_LIST(F)          \
    F(cur_el_sp0_sync)       \
    F(cur_el_sp0_irq)        \
    F(cur_el_sp0_fiq)        \
    F(cur_el_sp0_serror)     \
    F(cur_el_spn_sync)       \
    F(cur_el_spn_irq)        \
    F(cur_el_spn_fiq)        \
    F(cur_el_spn_serror)     \
    F(low_el_aarch64_sync)   \
    F(low_el_aarch64_irq)    \
    F(low_el_aarch64_fiq)    \
    F(low_el_aarch64_serror) \
    F(low_el_aarch32_sync)   \
    F(low_el_aarch32_irq)    \
    F(low_el_aarch32_fiq)    \
    F(low_el_aarch32_serror)


#define INT_ENUM_ENTRY(name) INT_TYPE_##name,
enum int_type { INT_LIST(INT_ENUM_ENTRY) INT_TYPE__N };
#undef INT_ENUM_ENTRY

#define INT_ISR_PROTYPE(name) void int_##name##_isr(int64_t *);
INT_LIST(INT_ISR_PROTYPE)
#undef INT_ISR_PROTYPE

void int_dispatch(enum int_type type, int64_t *rs);

void int_register(enum int_type type, int_dispatch_fn dispatch);

void int_not_implemented_dispatch(int64_t *rs);

void int_init_dispatch_tab();

static inline uint64_t irq_lock() {
    uint64_t state = 0;
    __asm__ volatile(
        "mrs %[state], DAIF;"
        "msr daifset, #2;"
        : [state] "=r"(state)
        :);
    return state;
}

static inline void irq_unlock(uint64_t state) {
    __asm__ volatile("msr daif, %[state]" : : [state] "r"(state));
}

#endif  // __ASSEMBLER__

#endif  // KERNEL_INT_H_
