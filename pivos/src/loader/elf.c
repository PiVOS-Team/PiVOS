#include <kernel/loader/elf.h>
#include <kernel/utils.h>

static const uint64_t s_elf_magic = 0x464C457F;

int32_t elf_read(struct elf_file* file) {
    struct elf_header* header = (struct elf_header*)file->bytes;

    if(header->e_ident.EI_MAG != s_elf_magic) {
        return 1;
    }

    if(header->e_ident.EI_CLASS != EI_CLASS_64_BIT) {
        return 1;
    }

    if(header->e_ident.EI_DATA != EI_DATA_LITTLE_ENDIAN) {
        return 1;
    }

    if(header->e_ident.EI_OSABI != EI_OSABI_System_V) {
        return 1;
    }

    if(header->e_phoff == 0 || header->e_phentsize == 0 || header->e_phnum == 0) {
        return 1;
    }

    file->elf_header = header;
    file->program_header = (struct elf_program_header*)(file->bytes + header->e_phoff);
    file->section_header = (struct elf_section_header*)(file->bytes + header->e_shoff);

    return 0;
}

int32_t elf_load(struct elf_file* file, struct memory_context* context, uint64_t* start_addr) {
    struct elf_program_header* phdr;
    for(uint32_t phdr_num = 0; phdr_num < file->elf_header->e_phnum; phdr_num++) {
        phdr = &file->program_header[phdr_num];
        
        if(phdr->p_type != PT_LOAD) {
            continue;
        }

        struct memory_region load_region = {
            .memory_type = MEMORY_TYPE_NORMAL_NO_CACHE,
            .va = phdr->p_vaddr,
            .size = phdr->p_memsz
        };

        memory_map_region(context, &load_region, MEMORY_MAP_ALLOC);
        
        if(phdr->p_filesz == 0) {
            continue;
        }

        void* data_src = (void*)((uint64_t)file->bytes + phdr->p_offset);
        bytecpy((void*)load_region.va, data_src, phdr->p_filesz);
    }

    *start_addr = file->elf_header->e_entry;
    return 0;
}