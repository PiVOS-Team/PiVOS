#include <kernel/alloc/bitmap.h>
#include <kernel/arch/mmu.h>
#include <kernel/config.h>
#include <kernel/mem.h>
#include <kernel/utils.h>

#define ALIGN_UP_PAGE(addr) ALIGN_UP(addr, MMU_PAGE_SIZE)
#define ALIGN_DOWN_PAGE(addr) ALIGN_DOWN(addr, MMU_PAGE_SIZE)

extern char mmu_space_start;
extern char kernel_end;
static struct mem_ctx s_kernel_memory_context;
static uint64_t s_max_allocatable_addr;

static void* get_phys_space_alloc(struct arch_mmu_space_req* req) {
    return alloc_bitmap_claim_pages(req->page_count, req->align);
}

static void* get_phys_space_one_to_one(struct arch_mmu_space_req* req) {
    return (void*)req->virt_addr;
}

static void* get_phys_space_one_to_one_and_alloc(struct arch_mmu_space_req* req) {
    alloc_bitmap_claim_range((void*)req->virt_addr, (void*)(req->virt_addr + (req->page_count * MMU_PAGE_SIZE)));
    return (void*)req->virt_addr;
}

static void* (*const get_phys_space[3])(struct arch_mmu_space_req* req) = {
    get_phys_space_alloc,
    get_phys_space_one_to_one,
    get_phys_space_one_to_one_and_alloc};

int32_t mem_init(uint64_t max_allocatable_address) {
    s_max_allocatable_addr = max_allocatable_address;
    uint64_t number_of_pages = s_max_allocatable_addr / MMU_PAGE_SIZE;
    uint64_t image_size = ((uint64_t)&kernel_end) & ADDRESS_MASK;

    // Setup page allocator
    alloc_bitmap_init((void*)ALIGN_UP_PAGE((uint64_t)&kernel_end), number_of_pages, MMU_PAGE_SIZE);

    // Claim kernel region
    alloc_bitmap_claim_range((void*)0, (void*)ALIGN_UP_PAGE(image_size));

    // Init mmu
    s_kernel_memory_context = (struct mem_ctx){.table = (uint64_t*)&mmu_space_start};
    arch_mmu_init(alloc_bitmap_claim_pages, alloc_bitmap_free_pages);

    return 1;
}

void mem_create_ctx(struct mem_ctx* ctx) {
    ctx->table = arch_mmu_create_table();
}

void mem_destroy_ctx(struct mem_ctx* ctx) {
    arch_mmu_destroy_table(ctx->table);
}

void mem_map_reg(struct mem_ctx* ctx, struct mem_reg* mem_reg, enum mem_map_type type) {
    arch_mmu_insert(ctx->table, mem_reg->base, mem_reg->size, mem_reg->mem_type, get_phys_space[type]);
}

void mem_set_usr_ctx(struct mem_ctx* ctx, uint16_t id) {
    arch_mmu_change_usr_ctx(ctx->table, id);
}

void mem_get_usr_ctx(struct mem_ctx* ctx) {
    ctx->table = arch_mmu_get_usr_ctx();
}

void mem_get_kernel_ctx(struct mem_ctx* ctx) {
    ctx->table = s_kernel_memory_context.table;
}