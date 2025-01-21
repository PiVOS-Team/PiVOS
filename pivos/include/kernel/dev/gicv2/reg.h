#ifndef KERNEL_DEV_GICV2_REG_H_
#define KERNEL_DEV_GICV2_REG_H_

#include <stdint.h>

#include <kernel/utils.h>

#ifndef GICV2_ADDRESS
#define GICV2_ADDRESS 0xFF840000
#endif  // GICV2_ADDRESS

struct bit32 {
    uint8_t B0  : 1;
    uint8_t B1  : 1;
    uint8_t B3  : 1;
    uint8_t B4  : 1;
    uint8_t B5  : 1;
    uint8_t B6  : 1;
    uint8_t B7  : 1;
    uint8_t B8  : 1;
    uint8_t B9  : 1;
    uint8_t B10 : 1;
    uint8_t B11 : 1;
    uint8_t B12 : 1;
    uint8_t B13 : 1;
    uint8_t B14 : 1;
    uint8_t B15 : 1;
    uint8_t B16 : 1;
    uint8_t B17 : 1;
    uint8_t B18 : 1;
    uint8_t B19 : 1;
    uint8_t B20 : 1;
    uint8_t B21 : 1;
    uint8_t B22 : 1;
    uint8_t B23 : 1;
    uint8_t B24 : 1;
    uint8_t B25 : 1;
    uint8_t B26 : 1;
    uint8_t B27 : 1;
    uint8_t B28 : 1;
    uint8_t B29 : 1;
    uint8_t B30 : 1;
    uint8_t B31 : 1;
};

struct dibit32 {
    uint8_t B0  : 2;
    uint8_t B1  : 2;
    uint8_t B2  : 2;
    uint8_t B3  : 2;
    uint8_t B4  : 2;
    uint8_t B5  : 2;
    uint8_t B6  : 2;
    uint8_t B7  : 2;
    uint8_t B8  : 2;
    uint8_t B9  : 2;
    uint8_t B10 : 2;
    uint8_t B11 : 2;
    uint8_t B12 : 2;
    uint8_t B13 : 2;
    uint8_t B14 : 2;
    uint8_t B15 : 2;
};

struct byte32 {
    uint8_t B0 : 8;
    uint8_t B1 : 8;
    uint8_t B2 : 8;
    uint8_t B3 : 8;
};

struct __attribute__((packed)) gicv {};

struct __attribute__((packed)) gich {};

union gicc_dir {
    volatile uint32_t bits;
    struct __attribute__((packed)) {

    } fields;
};

union gicc_iidr {
    volatile uint32_t bits;
    struct __attribute__((packed)) {

    } fields;
};

union gicc_nsapr0 {
    volatile uint32_t bits;
    struct __attribute__((packed)) {

    } fields;
};

union gicc_apr0 {
    volatile uint32_t bits;
    struct __attribute__((packed)) {

    } fields;
};

union gicc_ahppir {
    volatile uint32_t bits;
    struct __attribute__((packed)) {

    } fields;
};

union gicc_aeoir {
    volatile uint32_t bits;
    struct __attribute__((packed)) {

    } fields;
};

union gicc_aiar {
    volatile uint32_t bits;
    struct __attribute__((packed)) {

    } fields;
};

union gicc_abpr {
    volatile uint32_t bits;
    struct __attribute__((packed)) {

    } fields;
};

union gicc_hppir {
    volatile uint32_t bits;
    struct __attribute__((packed)) {

    } fields;
};

union gicc_rpr {
    volatile uint32_t bits;
    struct __attribute__((packed)) {

    } fields;
};

union gicc_eoir {
    volatile uint32_t bits;
    struct __attribute__((packed)) {
        uint16_t EOIINTID   : 10;
        uint8_t CPUID       : 3;
        const uint32_t RES0 : 19;
    } fields;
};

union gicc_iar {
    volatile uint32_t bits;
    struct __attribute__((packed)) {
        uint16_t InterruptID : 10;
        uint8_t CPUID        : 3;
        const uint32_t RES0  : 19;
    } fields;
};

union gicc_bpr {
    volatile uint32_t bits;
    struct __attribute__((packed)) {

    } fields;
};

union gicc_pmr {
    volatile uint32_t bits;
    struct __attribute__((packed)) {
        uint8_t Priority    : 8;
        const uint32_t RES0 : 24;
    } fields;
};

union gicc_ctlr {
    volatile uint32_t bits;
    // GICv2 with Security Extensions - Non-Secure copy
    struct __attribute__((packed)) {
        uint8_t EnableGrp1    : 1;
        const uint8_t RES0    : 4;
        uint8_t FIQBypDisGrp1 : 1;
        uint8_t IRQBypDisGrp1 : 1;
        const uint8_t RES1    : 2;
        uint8_t EOImodeNS     : 1;
        const uint32_t RES2   : 22;
    } fields_gicv2_with_SE_NS_copy;
};

struct __attribute__((packed)) gicc {
    union gicc_ctlr CTLR;
    union gicc_pmr PMR;
    union gicc_bpr BPR;
    union gicc_iar IAR;
    union gicc_eoir EOIR;
    union gicc_rpr RPR;
    union gicc_hppir HPPIR;
    union gicc_abpr ABPR;
    union gicc_aiar AIAR;
    union gicc_aeoir AEOIR;
    union gicc_ahppir AHPPIR;
    uint8_t RES0[0xD0 - 0x28 - sizeof(union gicc_ahppir)];
    union gicc_apr0 APR0;
    uint8_t RES1[0xE0 - 0xD0 - sizeof(union gicc_apr0)];
    union gicc_nsapr0 NSAPR0;
    uint8_t RES2[0xFC - 0xE0 - sizeof(union gicc_nsapr0)];
    union gicc_iidr IIDR;
    uint8_t RES3[0x1000 - 0xFC - sizeof(union gicc_iidr)];
    union gicc_dir DIR;
};

static_assert(offsetof(struct gicc, CTLR) == 0x0000, "Invalid offset");
static_assert(offsetof(struct gicc, PMR) == 0x0004, "Invalid offset");
static_assert(offsetof(struct gicc, BPR) == 0x0008, "Invalid offset");
static_assert(offsetof(struct gicc, IAR) == 0x000C, "Invalid offset");
static_assert(offsetof(struct gicc, EOIR) == 0x0010, "Invalid offset");
static_assert(offsetof(struct gicc, RPR) == 0x0014, "Invalid offset");
static_assert(offsetof(struct gicc, HPPIR) == 0x0018, "Invalid offset");
static_assert(offsetof(struct gicc, ABPR) == 0x001C, "Invalid offset");
static_assert(offsetof(struct gicc, AIAR) == 0x0020, "Invalid offset");
static_assert(offsetof(struct gicc, AEOIR) == 0x0024, "Invalid offset");
static_assert(offsetof(struct gicc, AHPPIR) == 0x0028, "Invalid offset");
static_assert(offsetof(struct gicc, APR0) == 0x00D0, "Invalid offset");
static_assert(offsetof(struct gicc, NSAPR0) == 0x00E0, "Invalid offset");
static_assert(offsetof(struct gicc, IIDR) == 0x00FC, "Invalid offset");
static_assert(offsetof(struct gicc, DIR) == 0x1000, "Invalid offset");

union gicd_cidr3 {
    volatile uint32_t bits;
    struct __attribute__((packed)) {

    } fields;
};

union gicd_cidr2 {
    volatile uint32_t bits;
    struct __attribute__((packed)) {

    } fields;
};

union gicd_cidr1 {
    volatile uint32_t bits;
    struct __attribute__((packed)) {

    } fields;
};

union gicd_cidr0 {
    volatile uint32_t bits;
    struct __attribute__((packed)) {

    } fields;
};

union gicd_pidr3 {
    volatile uint32_t bits;
    struct __attribute__((packed)) {

    } fields;
};

union gicd_pidr2 {
    volatile uint32_t bits;
    struct __attribute__((packed)) {

    } fields;
};

union gicd_pidr1 {
    volatile uint32_t bits;
    struct __attribute__((packed)) {

    } fields;
};

union gicd_pidr0 {
    volatile uint32_t bits;
    struct __attribute__((packed)) {

    } fields;
};

union gicd_pidr7 {
    volatile uint32_t bits;
    struct __attribute__((packed)) {

    } fields;
};

union gicd_pidr6 {
    volatile uint32_t bits;
    struct __attribute__((packed)) {

    } fields;
};

union gicd_pidr5 {
    volatile uint32_t bits;
    struct __attribute__((packed)) {

    } fields;
};

union gicd_pidr4 {
    volatile uint32_t bits;
    struct __attribute__((packed)) {

    } fields;
};

union gicd_spendsgir {
    volatile uint32_t bits;
    struct __attribute__((packed)) {

    } fields;
};

union gicd_cpendsgir {
    volatile uint32_t bits;
    struct __attribute__((packed)) {

    } fields;
};

union gicd_sgir {
    volatile uint32_t bits;
    struct __attribute__((packed)) {

    } fields;
};

union gicd_spisr {
    volatile uint32_t bits;
    struct __attribute__((packed)) {

    } fields;
};

union gicd_ppisr {
    volatile uint32_t bits;
    struct __attribute__((packed)) {

    } fields;
};

union gicd_icfgr {
    volatile uint32_t bits;
    struct dibit32 fields;
};

union gicd_itargetsr {
    volatile uint32_t bits;
    struct byte32 fields;
};

union gicd_ipriorityr {
    volatile uint32_t bits;
    struct byte32 fields;
};

union gicd_icactiver {
    volatile uint32_t bits;
    struct bit32 fields;
};

union gicd_isactiver {
    volatile uint32_t bits;
    struct bit32 fields;
};

union gicd_icpendr {
    volatile uint32_t bits;
    struct bit32 fields;
};

union gicd_ispendr {
    volatile uint32_t bits;
    struct bit32 fields;
};

union gicd_icenabler {
    volatile uint32_t bits;
    struct bit32 fields;
};

union gicd_isenabler {
    volatile uint32_t bits;
    struct bit32 fields;
};

union gicd_igroupr {
    volatile uint32_t bits;
    struct bit32 fields;
};

union gicd_iidr {
    volatile uint32_t bits;
    struct __attribute__((packed)) {

    } fields;
};

union gicd_typer {
    volatile uint32_t bits;
    struct __attribute__((packed)) {
        const uint8_t ITLinesNumber : 5;
        const uint8_t CPUNumber     : 3;
        const uint8_t RES0          : 2;
        const uint8_t SecurityExtn  : 1;
        const uint8_t LSPI          : 5;
        const uint16_t RES1         : 16;
    } fields;
};

union gicd_ctlr {
    volatile uint32_t bits;
    struct __attribute__((packed)) {
        uint8_t ENABLE      : 1;
        const uint32_t RES0 : 31;
    } fields;
};

struct __attribute__((packed)) gicd {
    union gicd_ctlr CTLR;
    union gicd_typer TYPER;
    union gicd_iidr IIDR;
    uint8_t RES0[0x080 - 0x008 - sizeof(union gicd_iidr)];
    union gicd_igroupr IGROUPR[32];
    union gicd_isenabler ISENABLER[32];
    union gicd_icenabler ICENABLER[32];
    union gicd_ispendr ISPENDR[32];
    union gicd_icpendr ICPENDR[32];
    union gicd_isactiver ISACTIVER[32];
    union gicd_icactiver ICACTIVER[32];
    union gicd_ipriorityr IPRIORITYR[256];
    union gicd_itargetsr ITARGETSR[256];
    union gicd_icfgr ICFGR[64];
    union gicd_ppisr PPISR;
    union gicd_spisr SPISR;
    uint8_t RES1[0xF00 - 0xD04 - sizeof(union gicd_spisr)];
    union gicd_sgir SGIR;
    uint8_t RES2[0xF10 - 0xF00 - sizeof(union gicd_sgir)];
    union gicd_cpendsgir CPENDSGIR;
    uint8_t RES3[0xF20 - 0xF10 - sizeof(union gicd_cpendsgir)];
    union gicd_spendsgir SPENDSGIR;
    uint8_t RES4[0xFD0 - 0xF20 - sizeof(union gicd_spendsgir)];
    union gicd_pidr4 PIDR4;
    union gicd_pidr5 PIDR5;
    union gicd_pidr6 PIDR6;
    union gicd_pidr7 PIDR7;
    union gicd_pidr0 PIDR0;
    union gicd_pidr1 PIDR1;
    union gicd_pidr2 PIDR2;
    union gicd_pidr3 PIDR3;
    union gicd_cidr0 CIDR0;
    union gicd_cidr1 CIDR1;
    union gicd_cidr2 CIDR2;
    union gicd_cidr3 CIDR3;
};

static_assert(offsetof(struct gicd, CTLR) == 0x000, "Invalid offset");
static_assert(offsetof(struct gicd, TYPER) == 0x004, "Invalid offset");
static_assert(offsetof(struct gicd, IIDR) == 0x008, "Invalid offset");
static_assert(offsetof(struct gicd, IGROUPR) == 0x080, "Invalid offset");
static_assert(offsetof(struct gicd, ISENABLER) == 0x100, "Invalid offset");
static_assert(offsetof(struct gicd, ICENABLER) == 0x180, "Invalid offset");
static_assert(offsetof(struct gicd, ISPENDR) == 0x200, "Invalid offset");
static_assert(offsetof(struct gicd, ICPENDR) == 0x280, "Invalid offset");
static_assert(offsetof(struct gicd, ISACTIVER) == 0x300, "Invalid offset");
static_assert(offsetof(struct gicd, ICACTIVER) == 0x380, "Invalid offset");
static_assert(offsetof(struct gicd, IPRIORITYR) == 0x400, "Invalid offset");
static_assert(offsetof(struct gicd, ITARGETSR) == 0x800, "Invalid offset");
static_assert(offsetof(struct gicd, ICFGR) == 0xC00, "Invalid offset");
static_assert(offsetof(struct gicd, PPISR) == 0xD00, "Invalid offset");
static_assert(offsetof(struct gicd, SPISR) == 0xD04, "Invalid offset");
static_assert(offsetof(struct gicd, SGIR) == 0xF00, "Invalid offset");
static_assert(offsetof(struct gicd, CPENDSGIR) == 0xF10, "Invalid offset");
static_assert(offsetof(struct gicd, SPENDSGIR) == 0xF20, "Invalid offset");
static_assert(offsetof(struct gicd, PIDR4) == 0xFD0, "Invalid offset");
static_assert(offsetof(struct gicd, PIDR5) == 0xFD4, "Invalid offset");
static_assert(offsetof(struct gicd, PIDR6) == 0xFD8, "Invalid offset");
static_assert(offsetof(struct gicd, PIDR7) == 0xFDC, "Invalid offset");
static_assert(offsetof(struct gicd, PIDR0) == 0xFE0, "Invalid offset");
static_assert(offsetof(struct gicd, PIDR1) == 0xFE4, "Invalid offset");
static_assert(offsetof(struct gicd, PIDR2) == 0xFE8, "Invalid offset");
static_assert(offsetof(struct gicd, PIDR3) == 0xFEC, "Invalid offset");
static_assert(offsetof(struct gicd, CIDR0) == 0xFF0, "Invalid offset");
static_assert(offsetof(struct gicd, CIDR1) == 0xFF4, "Invalid offset");
static_assert(offsetof(struct gicd, CIDR2) == 0xFF8, "Invalid offset");
static_assert(offsetof(struct gicd, CIDR3) == 0xFFC, "Invalid offset");

struct __attribute__((aligned(0x8000), packed)) gicv2 {
    const uint8_t RES0[0x1000];
    struct gicd __attribute__((aligned(0x1000))) GICD;
    struct gicc __attribute__((aligned(0x2000))) GICC;
    struct gich __attribute__((aligned(0x2000))) GICH;
    struct gicv __attribute__((aligned(0x2000))) GICV;
};

static_assert(offsetof(struct gicv2, GICD) == 0x1000, "Invalid offset");
static_assert(offsetof(struct gicv2, GICC) == 0x2000, "Invalid offset");
// static_assert(offsetof(gicv2_t, GICH)       == 0x4000);
// static_assert(offsetof(gicv2_t, GICV)       == 0x6000);

#endif  // KERNEL_DEV_GICV2_REG_H_