#include <kernel/memory.h>
#include <kernel/memory/allocator.h>
#include <kernel/memory/mmu.h>
#include <kernel/memory/mmu/reg.h>
#include <kernel/utils.h>

#define ALIGN_UP_PAGE(addr) ALIGN_UP(addr, MMU_PAGE_SIZE)
#define ALIGN_DOWN_PAGE(addr) ALIGN_DOWN(addr, MMU_PAGE_SIZE)

extern char mmu_space_start;
extern char kernel_end;
static struct memory_context s_kernel_memory_context;

static void* get_phys_space_alloc(struct mmu_phys_space_request* request) {
    return memory_allocator_claim_pages(request->page_count, request->align);
}

static void* get_phys_space_one_to_one(struct mmu_phys_space_request* request) {
    return (void*)request->virt_addr;
}

static const memory_mmu_get_phys_space_callback get_phys_space[2] = {
    get_phys_space_alloc,
    get_phys_space_one_to_one
};

int32_t memory_init(uint64_t size) {
    uint64_t number_of_pages = size / MMU_PAGE_SIZE;
    uint64_t peripherials_start = 0xF0000000ULL;
    uint64_t peripherials_end = 0x100000000ULL;
    uint64_t image_size = ((uint64_t)&kernel_end) & ~KERNEL_SPACE_MASK;

    ASSERT(LOW, &mmu_space_start > 0, "Check mmu_space_start in .ld script");
    ASSERT(LOW, &kernel_end > 0, "Check kernel_end symbol in .ld script");

    // Prepare allocator

    memory_allocator_init((void*)ALIGN_UP_PAGE((uint64_t)&kernel_end), number_of_pages, MMU_PAGE_SIZE);
    memory_allocator_claim_range((void*)0, (void*)ALIGN_UP_PAGE(image_size));
    memory_allocator_claim_range((void*)ALIGN_DOWN_PAGE(peripherials_start), (void*)ALIGN_UP_PAGE(peripherials_end));

    memory_mmu_init(memory_allocator_claim_pages, memory_allocator_free_pages);

    // Prepare kernel regions

    s_kernel_memory_context = (struct memory_context){
        .level0_table = (uint64_t*)&mmu_space_start
    };

    struct memory_region normal_memory_region_1 = {
        .memory_type = MEMORY_TYPE_NORMAL_NO_CACHE, 
        .va = 0x0,
        .size = peripherials_start
    };

    struct memory_region peripherals_region = {
        .memory_type = MEMORY_TYPE_DEVICE,
        .va = peripherials_start,
        .size = peripherials_end - peripherials_start
    };

    struct memory_region normal_memory_region_2 = {
        .memory_type = MEMORY_TYPE_NORMAL_NO_CACHE,
        .va = peripherials_end,
        .size = size - peripherials_end
    };

    memory_map_region(&s_kernel_memory_context, &normal_memory_region_1, MEMORY_MAP_ONE_TO_ONE);

    memory_map_region(&s_kernel_memory_context, &peripherals_region, MEMORY_MAP_ONE_TO_ONE);

    memory_map_region(&s_kernel_memory_context, &normal_memory_region_2, MEMORY_MAP_ONE_TO_ONE);

    return 0;
}

void memory_create_context(struct memory_context* context) {
    context->level0_table = memory_mmu_create_table();
}

void memory_destroy_context(struct memory_context* context) {
    memory_mmu_destroy_table(context->level0_table);
}

void memory_map_region(struct memory_context* context, struct memory_region* region, enum memory_map_type map_type) {
    memory_mmu_insert(context->level0_table, 0, region->va, region->size, region->memory_type, get_phys_space[map_type]);
}