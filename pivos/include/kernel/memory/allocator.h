#ifndef KERNEL_MEMORY_ALLOCATOR_H_
#define KERNEL_MEMORY_ALLOCATOR_H_

#include <stdint.h>

void memory_allocator_init(void *addr, uint32_t number_of_pages,
                           uint32_t page_size);

void *memory_allocator_claim_page();

void memory_allocator_claim_range(void *from, void *to);

void *memory_allocator_claim_pages(uint32_t number, uint32_t align);

void memory_allocator_free_page(void *addr);

void memory_allocator_free_range(void *from, void *to);

void memory_allocator_free_pages(void *addr, uint32_t number);

#endif  // KERNEL_MEMORY_ALLOCATOR_H_