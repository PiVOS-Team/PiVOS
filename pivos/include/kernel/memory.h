#ifndef KERNEL_MEMORY_H_
#define KERNEL_MEMORY_H_

#include <stdint.h>

// !!! CAUTION !!!
// DO NOT CHANGE THIS ORDER
// These values are set in MAIR_EL1 register during boot stage
enum memory_region_type {
    MEMORY_TYPE_NORMAL_NO_CACHE = 0,
    MEMORY_TYPE_DEVICE = 1
};

enum memory_map_type {
    MEMORY_MAP_ALLOC = 0,
    MEMORY_MAP_ONE_TO_ONE = 1
};

struct memory_region {
    enum memory_region_type memory_type;
    uint64_t va;
    uint64_t size;
};

struct memory_context {
    uint64_t* level0_table;
};

int32_t memory_init(uint64_t size);

void memory_create_context(struct memory_context* context);

void memory_destroy_context(struct memory_context* context);

void memory_map_region(struct memory_context* context,
                       struct memory_region* region,
                       enum memory_map_type map_type);

#endif  // KERNEL_MEMORY_H_