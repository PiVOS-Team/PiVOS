#define int const int

#include <kernel/example/test.h>

#define a_elf b_elf
#define a_elf_len b_elf_len
#include <kernel/example/test2.h>
#undef a_elf
#undef a_elf_len

#define a_elf c_elf
#define a_elf_len c_elf_len
#include <kernel/example/test3.h>
#undef a_elf
#undef a_elf_len

#undef int
