#include <stddef.h>

#include <kernel/int.h>

int_dispatch_fn int_dispatch_tab[INT_TYPE__N];

void int_dispatch(enum int_type type, int64_t *rs) {
    void (*dispatch)(int64_t *) = int_dispatch_tab[type];
    dispatch(rs);
}

void int_register(enum int_type type, int_dispatch_fn dispatch) {
    int_dispatch_tab[type] = dispatch;
}

void int_not_implemented_dispatch(int64_t *rs) {
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
