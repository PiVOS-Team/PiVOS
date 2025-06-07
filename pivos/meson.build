project('pivos', 'c',
    version : '0.0.1',
    meson_version : '>=1.1',
    default_options : []
)

kernel_virtual_addr = get_option('kernel_virtual_addr')
kernel_physical_addr = get_option('kernel_physical_addr')
gdb_debug_port = get_option('gdb_debug_port')
gdb_server_address = get_option('gdb_server_address')
board = get_option('board')
cpu_model = get_option('cpu_model')
img_output = 'kernel8.img'

cc = find_program('cc')
objcopy = find_program('objcopy')
qemu = find_program('qemu', required : true)
gdb = find_program(['gdb-multiarch', 'gdb'], required : true)


fill_cmd = ['scripts/fill_script.sh', '@INPUT@', '@OUTPUT@']

c_args = [
    '-ffreestanding',
    '-nostartfiles',
    '-mgeneral-regs-only',
]

link_args = [
    '-nostdlib'
]

headers = include_directories('include')

sources = files(
    'src/boot.s',
    'src/libc.c',
    'src/dev.c',
    'src/main.c',
    'src/int.c',
    'src/int/int_table.s',
    'src/dev/uart.c',
    'src/dev/gicv2.c',
    'src/dev/generic_timer.c',
)

ldscript = custom_target(
    'ldscript',
    input : 'scripts/link.ld.in',
    output : 'link.ld',
    command : fill_cmd,
    env : {
        'ENTRY_OFFSET': kernel_physical_addr,
    },
    build_by_default : false,
)

link_args += [
    '-T', ldscript.full_path()
]

elf = executable(
    'pivos_elf',
    sources,
    c_args : c_args,
    link_args : link_args,
    include_directories : headers,
    link_depends : [ldscript]
)

img = custom_target(
    'kernel8',
    output : img_output,
    input : elf,
    command : [objcopy, '@INPUT@', '-O', 'binary', '@OUTPUT@'],
    build_by_default : false
)

alias_target('build_sources', [ldscript, img])

tools_env = {
    'PROJECT_ROOT_': meson.project_source_root(),
    'GDB_PATH_': gdb.full_path(),
    'GDB_PORT_' : gdb_debug_port,
    'GDB_SERVER_' : gdb_server_address,
    'QEMU_PATH_': qemu.full_path(),
    'QEMU_BOARD_': board,
    'QEMU_CPU_': cpu_model,
    'IMG_NAME_': img_output,
    'ELF_NAME_': elf.name(),
    'KERNEL_PHYS_ADDR_': kernel_physical_addr,
    'KERNEL_VIRT_ADDR_': kernel_virtual_addr
}

env_out = custom_target(
    'env',
    input : 'scripts/.env.in',
    output : '.env',
    command : fill_cmd,
    env : tools_env,
    build_by_default : false
)

launch = custom_target(
    'launch_json',
    input : 'scripts/launch.json.in',
    output : 'launch.json',
    command : fill_cmd,
    env : tools_env,
    build_by_default : false
)

alias_target('build_tools', [env_out, launch])