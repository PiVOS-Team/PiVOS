#ifndef ARCH_ARM64_INT_H_
#define ARCH_ARM64_INT_H_

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

#ifndef __ASSEMBLER__

#include <stdint.h>

extern void vector_table();

#define INT_ISR_PROTYPE(name) void int_##name##_isr(int64_t *);
INT_LIST(INT_ISR_PROTYPE)
#undef INT_ISR_PROTYPE

#endif  // __ASSEMBLER__

#endif