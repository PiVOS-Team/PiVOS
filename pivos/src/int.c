#include <stddef.h>

#include <kernel/int.h>

int_dispatch_fn int_dispatch_tab[INT_TYPE__N];

int test_func1() {
    return 0;
}

void int_dispatch(enum int_type type, int64_t *rs) {
    uint64_t esr_el1;
    int_dispatch_fn dispatch = int_dispatch_tab[type];
    __asm__ volatile("mrs %[esr_el1], esr_el1" : [esr_el1] "=r"(esr_el1));
    dispatch(rs, esr_el1);
    test_func1();
}

void int_register(enum int_type type, int_dispatch_fn dispatch) {
    int_dispatch_tab[type] = dispatch;
}

void int_not_implemented_dispatch(int64_t *rs, uint64_t esr) {
    // TODO: should probably die
    return;
}

void int_init_dispatch_tab() {
    for (enum int_type i = 0; i < INT_TYPE__N; i++) {
        int_dispatch_tab[i] = int_not_implemented_dispatch;
    }
    __asm__ volatile("msr vbar_el1, %[tab]" : : [tab] "r"(vector_table) :);
}

#define INT_ISR_IMPL(name)                 \
    void int_##name##_isr(int64_t *rs) {   \
        int_dispatch(INT_TYPE_##name, rs); \
    }
INT_LIST(INT_ISR_IMPL)
#undef INT_ISR_IMPL
