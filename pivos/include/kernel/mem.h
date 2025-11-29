#ifndef KERNEL_MEM_H_
#define KERNEL_MEM_H_

#include <stdint.h>

enum mem_map_type {
    MEM_MAP_ALLOC = 0,
    MEM_MAP_ONE_TO_ONE = 1,
    MEM_MAP_ONE_TO_ONE_AND_ALLOC = 2
};

enum mem_reg_type {
    MEM_TYPE_NORMAL_NO_CACHE = 0,
    MEM_TYPE_DEVICE = 1,
};

struct mem_reg {
    enum mem_reg_type mem_type;
    uint64_t base;
    uint64_t size;
};

typedef void* mem_ctx;

struct mem_ctx {
    void* table;
};

int32_t mem_init(uint64_t max_allocatable_address);

void mem_create_ctx(struct mem_ctx* ctx);

void mem_destroy_ctx(struct mem_ctx* ctx);

void mem_map_reg(struct mem_ctx* ctx, struct mem_reg* mem_reg, enum mem_map_type type);

void mem_set_usr_ctx(struct mem_ctx* ctx, uint16_t id);

void mem_get_usr_ctx(struct mem_ctx* ctx);

void mem_get_kernel_ctx(struct mem_ctx* ctx);

#endif
