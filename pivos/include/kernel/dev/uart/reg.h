#ifndef KERNEL_DEV_UART_REG_H_
#define KERNEL_DEV_UART_REG_H_

#include <kernel/utils.h>
#include <stdint.h>

#ifndef UART_ADDRESS
#define UART_ADDRESS (0xfe201000UL + KERNEL_SPACE_MASK)
#endif  // UART_ADDRESS

union uart_dr {
    volatile uint32_t bits;
    struct __attribute__((packed)) {
        uint32_t DATA       : 8;
        const uint32_t FE   : 1;
        const uint32_t PE   : 1;
        const uint32_t BE   : 1;
        const uint32_t OE   : 1;
        const uint32_t RES0 : 20;
    } fields;
};

union uart_rsrecr {
    volatile uint32_t bits;
    struct __attribute__((packed)) {
        uint32_t FE         : 1;
        uint32_t PE         : 1;
        uint32_t BE         : 1;
        uint32_t OE         : 1;
        const uint32_t RES0 : 28;
    } fields;
};

union uart_fr {
    const volatile uint32_t bits;
    struct __attribute__((packed)) {
        const uint32_t CTS  : 1;
        const uint32_t DSR  : 1;
        const uint32_t DCD  : 1;
        const uint32_t BUSY : 1;
        const uint32_t RXFE : 1;
        const uint32_t TXFF : 1;
        const uint32_t RXFF : 1;
        const uint32_t TXFE : 1;
        const uint32_t RI   : 1;
        const uint32_t RES0 : 23;
    } fields;
};

union uart_ilpr {
    const volatile uint32_t bits;
    struct __attribute__((packed)) {
        // UNUSED
    } fields;
};

union uart_ibrd {
    volatile uint32_t bits;
    struct __attribute__((packed)) {
        uint32_t IBRD       : 16;
        const uint32_t RES0 : 16;
    } fields;
};

union uart_fbrd {
    volatile uint32_t bits;
    struct __attribute__((packed)) {
        uint32_t FBRD       : 6;
        const uint32_t RES0 : 26;
    } fields;
};

union uart_lcrh {
    volatile uint32_t bits;
    struct __attribute__((packed)) {
        uint32_t BRK        : 1;
        uint32_t PEN        : 1;
        uint32_t EPS        : 1;
        uint32_t STP2       : 1;
        uint32_t FEN        : 1;
        uint32_t WLEN       : 2;
        uint32_t SPS        : 1;
        const uint32_t RES0 : 24;
    } fields;
};

union uart_cr {
    volatile uint32_t bits;
    struct __attribute__((packed)) {
        uint32_t UARTEN      : 1;
        const uint32_t SIREN : 1;
        const uint32_t SIRLP : 1;
        const uint32_t RES0  : 4;
        uint32_t LBE         : 1;
        uint32_t TXE         : 1;
        uint32_t RXE         : 1;
        const uint32_t DTR   : 1;
        uint32_t RTS         : 1;
        const uint32_t OUT1  : 1;
        const uint32_t OUT2  : 1;
        uint32_t RTSEN       : 1;
        uint32_t CTSEN       : 1;
        const uint32_t RES1  : 16;
    } fields;
};

union uart_ifls {
    volatile uint32_t bits;
    struct __attribute__((packed)) {
        uint32_t TXIFLSEL       : 3;
        uint32_t RXIFLSEL       : 3;
        const uint32_t TXIFPSEL : 3;
        const uint32_t RXIFPSEL : 3;
        const uint32_t RES0     : 20;
    } fields;
};

union uart_imsc {
    volatile uint32_t bits;
    struct __attribute__((packed)) {
        const uint32_t RIMIM  : 1;
        uint32_t CTSMIM       : 1;
        const uint32_t DCDMIM : 1;
        const uint32_t DSRMIM : 1;
        uint32_t RXIM         : 1;
        uint32_t TXIM         : 1;
        uint32_t RTIM         : 1;
        uint32_t FEIM         : 1;
        uint32_t PEIM         : 1;
        uint32_t BEIM         : 1;
        uint32_t OEIM         : 1;
        const uint32_t RES0   : 21;
    } fields;
};

union uart_ris {
    const volatile uint32_t bits;
    struct __attribute__((packed)) {
        const uint32_t RIRMIS  : 1;
        const uint32_t CTSRMIS : 1;
        const uint32_t DCDRMIS : 1;
        const uint32_t DSRRMIS : 1;
        const uint32_t RXRIS   : 1;
        const uint32_t TXRIS   : 1;
        const uint32_t RTRIS   : 1;
        const uint32_t FERIS   : 1;
        const uint32_t PERIS   : 1;
        const uint32_t BERIS   : 1;
        const uint32_t OERIS   : 1;
        const uint32_t RES0    : 21;
    } fields;
};

union uart_mis {
    const volatile uint32_t bits;
    struct __attribute__((packed)) {
        const uint32_t RIMMIS  : 1;
        const uint32_t CTSMMIS : 1;
        const uint32_t DCDMMIS : 1;
        const uint32_t DSRMMIS : 1;
        const uint32_t RXMIS   : 1;
        const uint32_t TXMIS   : 1;
        const uint32_t RTMIS   : 1;
        const uint32_t FEMIS   : 1;
        const uint32_t PEMIS   : 1;
        const uint32_t BEMIS   : 1;
        const uint32_t OEMIS   : 1;
        const uint32_t RES0    : 21;
    } fields;
};

union uart_icr {
    volatile uint32_t bits;
    struct __attribute__((packed)) {
        uint32_t RIMIC  : 1;
        uint32_t CTSMIC : 1;
        uint32_t DCDMIC : 1;
        uint32_t DSRMIC : 1;
        uint32_t RXIC   : 1;
        uint32_t TXIC   : 1;
        uint32_t RTIC   : 1;
        uint32_t FEIC   : 1;
        uint32_t PEIC   : 1;
        uint32_t BEIC   : 1;
        uint32_t OEIC   : 1;
        uint32_t RES0   : 21;
    } fields;
};

union uart_dmacr {
    volatile uint32_t bits;
    struct __attribute__((packed)) {
        uint32_t RXDMAE     : 1;
        uint32_t TXDMAE     : 1;
        uint32_t DMAONERR   : 1;
        const uint32_t RES0 : 29;
    } fields;
};

union uart_itcr {
    volatile uint32_t bits;
    struct __attribute__((packed)) {
        uint32_t ITCR0      : 1;
        uint32_t ITCR1      : 1;
        const uint32_t RES0 : 30;
    } fields;
};

union uart_itip {
    volatile uint32_t bits;
    struct __attribute__((packed)) {
        uint32_t ITIP0      : 1;
        const uint32_t RES0 : 2;
        uint32_t ITIP3      : 1;
        const uint32_t RES1 : 28;
    } fields;
};

union uart_itop {
    volatile uint32_t bits;
    struct __attribute__((packed)) {
        uint32_t ITOP0      : 1;
        const uint32_t RES0 : 2;
        uint32_t ITOP3      : 1;
        const uint32_t RES1 : 2;
        uint32_t ITOP6      : 1;
        uint32_t ITOP7      : 1;
        uint32_t ITOP8      : 1;
        uint32_t ITOP9      : 1;
        uint32_t ITOP10     : 1;
        uint32_t ITOP11     : 1;
        const uint32_t RES2 : 20;
    } fields;
};

union uart_tdr {
    volatile uint32_t bits;
    struct __attribute__((packed)) {
        uint32_t TDR10_0    : 11;
        const uint32_t RES0 : 21;
    } fields;
};

struct __attribute__((aligned(0x200), packed)) uart {
    union uart_dr DR;
    union uart_rsrecr RSRECR;
    const uint8_t RES0[0x10];
    union uart_fr FR;
    const uint8_t RES1[0x04];
    union uart_ilpr ILPR;
    union uart_ibrd IBRD;
    union uart_fbrd FBRD;
    union uart_lcrh LCRH;
    union uart_cr CR;
    union uart_ifls IFLS;
    union uart_imsc IMSC;
    union uart_ris RIS;
    union uart_mis MIS;
    union uart_icr ICR;
    union uart_dmacr DMACR;
    const uint8_t RES2[0x34];
    union uart_itcr ITCR;
    union uart_itip ITIP;
    union uart_itop ITOP;
    union uart_tdr TDR;
};

static_assert(offsetof(struct uart, DR) == 0x00, "Invalid offset");
static_assert(offsetof(struct uart, RSRECR) == 0x04, "Invalid offset");
static_assert(offsetof(struct uart, FR) == 0x18, "Invalid offset");
static_assert(offsetof(struct uart, ILPR) == 0x20, "Invalid offset");
static_assert(offsetof(struct uart, IBRD) == 0x24, "Invalid offset");
static_assert(offsetof(struct uart, FBRD) == 0x28, "Invalid offset");
static_assert(offsetof(struct uart, LCRH) == 0x2c, "Invalid offset");
static_assert(offsetof(struct uart, IFLS) == 0x34, "Invalid offset");
static_assert(offsetof(struct uart, IMSC) == 0x38, "Invalid offset");
static_assert(offsetof(struct uart, RIS) == 0x3c, "Invalid offset");
static_assert(offsetof(struct uart, MIS) == 0x40, "Invalid offset");
static_assert(offsetof(struct uart, ICR) == 0x44, "Invalid offset");
static_assert(offsetof(struct uart, DMACR) == 0x48, "Invalid offset");
static_assert(offsetof(struct uart, ITCR) == 0x80, "Invalid offset");
static_assert(offsetof(struct uart, ITIP) == 0x84, "Invalid offset");
static_assert(offsetof(struct uart, ITOP) == 0x88, "Invalid offset");
static_assert(offsetof(struct uart, TDR) == 0x8c, "Invalid offset");

#endif  // KERNEL_DEV_UART_REG_H_