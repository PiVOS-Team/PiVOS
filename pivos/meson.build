project('pivos', 'c',
    version : '0.0.1',
    meson_version : '>=1.1',
    default_options : [
        'buildtype=debugoptimized',
    ]
)

c_args = [
    '-ffreestanding',
    '-nostartfiles',
    '-mgeneral-regs-only',
]

link_args = [
    '-nostdlib'
]

entry_offset = get_option('entry_offset')
board = get_option('board')
cpu_model = get_option('cpu_model')

cc = find_program('cc')
objcopy = find_program('objcopy')
qemu = find_program('qemu', required : false)
gdb = find_program('gdb', required : false)

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
    command : ['scripts/fill_script.sh', '@INPUT@', '@OUTPUT@'],
    env : {
        'ENTRY_OFFSET': entry_offset.to_string(),
    },
    build_by_default : true,
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
    output : 'kernel8.img',
    input : elf,
    command : [objcopy, '@INPUT@', '-O', 'binary', '@OUTPUT@'],
    build_by_default : true
)

qemu_board = board

if qemu.found()

    qemu_env = {
        'QEMU': qemu.full_path(),
        'BOARD': qemu_board,
        'CPU': cpu_model,
        'IMG': img.full_path(),
    }

    fill_cmd = ['scripts/fill_script.sh', '@INPUT@', '@OUTPUT@']

    custom_target(
        'start_sh',
        input : 'scripts/start.sh.in',
        output : 'start.sh',
        command : fill_cmd,
        env : qemu_env,
        build_by_default : true,
    )

    if gdb.found()
        gdb_env = qemu_env + {
            'GDB': gdb.full_path(),
            'ELF': elf.full_path(),
        }

        custom_target(
            'debug_sh',
            input : 'scripts/dbg.sh.in',
            output : 'dbg.sh',
            command : fill_cmd,
            env : gdb_env,
            build_by_default : true,
        )
        custom_target(
            'connect_debug_sh',
            input : 'scripts/con_dbg.sh.in',
            output : 'con_dbg.sh',
            command : fill_cmd,
            env : gdb_env,
            build_by_default : true,
        )
    endif

endif
