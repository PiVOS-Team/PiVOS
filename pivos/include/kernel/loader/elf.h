#ifndef KERNEL_LOADER_ELF_H_
#define KERNEL_LOADER_ELF_H_

#include <stdint.h>
#include <kernel/memory/mmu.h>
#include <kernel/memory.h>

enum {
    EI_CLASS_32_BIT                         = 0x01,
    EI_CLASS_64_BIT                         = 0x02
};

enum {
    EI_DATA_LITTLE_ENDIAN                   = 0x01,
    EI_DATA_BIG_ENDIAN                      = 0x02
};

enum {
    EI_OSABI_System_V                       = 0x00,
    EI_OSABI_HP_UX                          = 0x01,
    EI_OSABI_NetBSD                         = 0x02,
    EI_OSABI_Linux                          = 0x03,
    EI_OSABI_GNU_Hurd                       = 0x04,
    EI_OSABI_Solaris                        = 0x06,
    EI_OSABI_AIX                            = 0x07,
    EI_OSABI_IRIX                           = 0x08,
    EI_OSABI_FreeBSD                        = 0x09,
    EI_OSABI_Tru64                          = 0x0A,
    EI_OSABI_Novell_Modesto                 = 0x0B,
    EI_OSABI_OpenBSD                        = 0x0C,
    EI_OSABI_OpenVMS                        = 0x0D,
    EI_OSABI_NonStop_Kernel                 = 0x0E,
    EI_OSABI_AROS                           = 0x0F,
    EI_OSABI_FenixOS                        = 0x10,
    EI_OSABI_Nuxi_CloudABI                  = 0x11,
    EI_OSABI_Stratus_Technologies_OpenVOS   = 0x12
};

enum {
    ET_NONE                                 = 0x00,
    ET_REL                                  = 0x01,
    ET_EXEC                                 = 0x02,
    ET_DYN                                  = 0x03,
    ET_CORE                                 = 0x04,
    ET_LOOS                                 = 0xFE00,
    ET_HIOS                                 = 0xFEFF,
    ET_LOPROC                               = 0xFF00,
    ET_HIPROX                               = 0xFFFF
};

enum {
    E_MACHINE_NO_SPECIFIC                   = 0x00,
    E_MACHINE_x86                           = 0x03,
    
    // ...
    
    E_MACHINE_ARM                           = 0x28,
    E_MACHINE_IA_64                         = 0x32,
    E_MACHINE_AMD_x86_64                    = 0x3E,
    
    // ...
    
    E_MACHINE_ARM64                         = 0xB7,
    E_MACHINE_RISC_V                        = 0xF3
    
    // ...
};

enum {
    PT_NULL                                 = 0x00000000,
    PT_LOAD                                 = 0x00000001,
    PT_DYNAMIC                              = 0x00000002,
    PT_INTERP                               = 0x00000003,
    PT_NOTE                                 = 0x00000004,
    PT_SHLIB                                = 0x00000005,
    PT_PHDR                                 = 0x00000006,
    PT_TLS                                  = 0x00000007,
    PT_LOOS                                 = 0x60000000,
    PT_HIOS                                 = 0x6FFFFFFF,
    PT_LOPROC                               = 0x70000000,
    PT_HIPROC                               = 0x7FFFFFFF
};

enum {
    PF_X                                    = 0x1,
    PF_W                                    = 0x2,
    PF_R                                    = 0x4
};

enum {
    SHT_NULL                                = 0x00,
    SHT_PROGBITS                            = 0x01,
    SHT_SYMTAB                              = 0x02,
    SHT_STRTAB                              = 0x03,
    SHT_RELA                                = 0x04,
    SHT_HASH                                = 0x05,
    SHT_DYNAMIC                             = 0x06,
    SHT_NOTE                                = 0x07,
    SHT_NOBITS                              = 0x08,
    SHT_REL                                 = 0x09,
    SHT_SHLIB                               = 0x0A,
    SHT_DYNSYM                              = 0x0B,
    SHT_INIT_ARRAY                          = 0x0E,
    SHT_FINI_ARRAY                          = 0x0F,
    SHT_PREINIT_ARRAY                       = 0x10,
    SHT_GROUP                               = 0x11,
    SHT_SYMTAB_SHNDX                        = 0x12,
    SHT_NUM                                 = 0x13,
    SHT_LOOS                                = 0x60000000
};

enum {
    SHF_WRITE                               = 0x1,
    SHF_ALLOC                               = 0x2,
    SHF_EXECINSTR                           = 0x4,
    SHF_MERGE                               = 0x10,
    SHF_STRINGS                             = 0x20,
    SHF_INFO_LINK                           = 0x40,
    SHF_LINK_ORDER                          = 0x80,
    SHF_OS_NONCONFORMING                    = 0x100,
    SHF_GROUP                               = 0x200,
    SHF_TLS                                 = 0x400,
    SHF_MASKOS                              = 0x0FF00000,
    SHF_MASKPROC                            = 0xF0000000,
    SHF_ORDERED                             = 0x4000000,
    SHF_EXCLUDE                             = 0x8000000
};

enum {
    ELF_OK,
    ELF_ERROR_INVALID_LENGTH,
    ELF_ERROR_INVALID_MAGIC,
    ELF_ERROR_UNSUPPORTED_CLASS,
    ELF_ERROR_UNSUPPORTED_DATA_TYPE,
    ELF_ERROR_UNSUPPORTED_ABI_MODE
};

struct elf_header_ident {
    uint32_t EI_MAG;
    uint8_t EI_CLASS;
    uint8_t EI_DATA;
    uint8_t EI_VERSION;
    uint8_t EI_OSABI;
    uint8_t EI_ABIVERSION;
    uint8_t EI_PAD[7];
} __attribute__((packed));

struct elf_header {
    struct elf_header_ident e_ident;
    uint16_t e_type;
    uint16_t e_machine;
    uint32_t e_version;
    uint64_t e_entry;
    uint64_t e_phoff;
    uint64_t e_shoff;
    uint32_t e_flags;
    uint16_t e_ehsize;
    uint16_t e_phentsize;
    uint16_t e_phnum;
    uint16_t e_shentsize;
    uint16_t e_shnum;
    uint16_t e_shstrndx;
} __attribute__((packed));

struct elf_program_header {
    uint32_t p_type;
    uint32_t p_flags;
    uint64_t p_offset;
    uint64_t p_vaddr;
    uint64_t p_paddr;
    uint64_t p_filesz;
    uint64_t p_memsz;
    uint64_t p_align;
} __attribute__((packed));

struct elf_section_header {
    uint32_t sh_name;
    uint32_t sh_type;
    uint64_t sh_flags;
    uint64_t sh_addr;
    uint64_t sh_offset;
    uint64_t sh_size;
    uint32_t sh_link;
    uint32_t sh_info;
    uint64_t sh_addralign;
    uint64_t sh_entsize;
} __attribute__((packed));

struct elf_symbol {
    uint32_t st_name;
    uint8_t st_info;
    uint8_t st_other;
    uint16_t st_shndx;
    uint64_t st_value;
    uint64_t st_size;
} __attribute__((packed));

struct elf_file {
    uint8_t* bytes;
    uint64_t length;
    struct elf_header* elf_header;
    struct elf_program_header* program_header;
    struct elf_section_header* section_header;
};

int32_t elf_read(struct elf_file* file);
int32_t elf_load(struct elf_file* file, struct memory_context* context, uint64_t* start_addr);

#endif