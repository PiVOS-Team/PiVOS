#ifndef KERNEL_DEV_GENERIC_TIMER_REG_H_
#define KERNEL_DEV_GENERIC_TIMER_REG_H_

#include <stdint.h>

union cntp_cval_el0_t {
    uint64_t bits;
    struct __attribute__((packed)) {
        uint64_t CompareValue;
    } fields;
};

union cntp_tval_el0_t {
    uint64_t bits;
    struct __attribute__((packed)) {
        uint32_t TimerValue;
        uint32_t RES0;
    } fields;
};

union cntp_ctl_el0_t {
    uint64_t bits;
    struct __attribute__((packed)) {
        uint8_t ENABLE  : 1;
        uint8_t IMASK   : 1;
        uint8_t ISTATUS : 1;
        uint32_t RES0   : 29;
    } fields;
};

union cntpct_el0_t {
    uint64_t bits;
    struct __attribute__((packed)) {
        uint64_t PhysicalCountValue;
    } fields;
};

union cntfrq_el0_t {
    uint32_t bits;
    struct __attribute__((packed)) {
        uint32_t ClockFrequency;
        uint32_t RES0;
    } fields;
};

#endif  // KERNEL_DEV_GENERIC_TIMER_REG_H_