project(
    'pivos',
    'c',
    version: '0.0.1',
    meson_version: '>=1.1',
    default_options: [],
)

if get_option('buildtype') == 'debug'
    add_project_arguments('-DDEBUG', '-DASSERT_LEVEL=3', '-DARM_MMU_GRANULE_SIZE_4KB', language: 'c')
endif

add_project_arguments('-DARM_MMU_GRANULE_SIZE_4KB', language: 'c')


kernel_virtual_mask = get_option('kernel_virtual_mask')
kernel_physical_addr = get_option('kernel_physical_addr')
gdb_debug_port = get_option('gdb_debug_port')
gdb_server_address = get_option('gdb_server_address')
board = get_option('board')
cpu_model = get_option('cpu_model')
img_name = 'kernel8.img'

cc = find_program('cc')
objcopy = find_program('objcopy')
qemu = find_program('qemu', required: true)
gdb = find_program(['gdb-multiarch', 'gdb'], required: true)

fill_cmd = ['scripts/fill_script.sh', '@INPUT@', '@OUTPUT@']

c_args = [
    '-ffreestanding',
    '-nostartfiles',
    '-mgeneral-regs-only',
]

link_args = ['-nostdlib']

headers = include_directories('include')

sources = files(
    'src/boot/boot.S',
    'src/boot/mmu.S',
    'src/memory.c',
    'src/memory/allocator.c',
    'src/memory/mmu.c',
    'src/dev.c',
    'src/dev/generic_timer.c',
    'src/dev/gicv2.c',
    'src/dev/uart.c',
    'src/int.c',
    'src/int/int_table.S',
    'src/libc.c',
    'src/main.c',
    'src/assert.c'
)

ldscript = custom_target(
    'ldscript',
    input: 'scripts/link.ld.in',
    output: 'link.ld',
    command: fill_cmd,
    env: {
        'KERNEL_PHYS_ADDR': kernel_physical_addr,
        'KERNEL_VIRT_MASK': kernel_virtual_mask
    },
    build_by_default: false,
)

link_args += ['-T', ldscript.full_path()]

elf = executable(
    'pivos_elf',
    sources,
    c_args: c_args,
    link_args: link_args,
    include_directories: headers,
    link_depends: [ldscript],
)

img = custom_target(
    'kernel8',
    output: img_name,
    input: elf,
    command: [objcopy, '@INPUT@', '-O', 'binary', '@OUTPUT@'],
    build_by_default: false,
)

alias_target('kernel', [ldscript, img])

env = {
    'PROJECT_ROOT': meson.project_source_root(),
    'GDB_PATH': gdb.full_path(),
    'GDB_PORT': gdb_debug_port,
    'GDB_SERVER': gdb_server_address,
    'QEMU_PATH': qemu.full_path(),
    'QEMU_BOARD': board,
    'QEMU_CPU': cpu_model,
    'IMG_PATH': img.full_path(),
    'ELF_PATH': elf.full_path(),
    'KERNEL_PHYS_ADDR': kernel_physical_addr,
    'KERNEL_VIRT_MASK': kernel_virtual_mask,
}

launch = custom_target(
    'launch_json',
    input: 'scripts/launch.json.in',
    output: 'launch.json',
    command: fill_cmd,
    env: env,
    build_by_default: false,
)

scripts = []

if qemu.found()
    scripts += custom_target(
        'start_sh',
        input: 'scripts/start.sh.in',
        output: 'start.sh',
        command: fill_cmd,
        env: env,
        build_by_default: false,
    )

    if gdb.found()
        scripts += custom_target(
            'debug_sh',
            input: 'scripts/dbg.sh.in',
            output: 'dbg.sh',
            command: fill_cmd,
            env: env,
            build_by_default: false,
        )
        scripts += custom_target(
            'connect_debug_sh',
            input: 'scripts/con_dbg.sh.in',
            output: 'con_dbg.sh',
            command: fill_cmd,
            env: env,
            build_by_default: false,
        )
    endif

endif

alias_target('tools', [launch] + scripts)