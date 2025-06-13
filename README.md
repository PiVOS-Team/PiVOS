# PiVOS
Pi Versatile Operating System

## Getting Started

### Prerequisites

- *Debian / Ubuntu / Raspberry*
```bash
sudo apt install qemu-system gdb-multiarch gcc-aarch64-linux-gnu meson ninja-build
```
- *Fedora*
```bash
sudo dnf install @virtualization gdb gcc-aarch64-linux-gnu meson ninja-build  
```
- *Arch*
```bash
sudo pacman -S qemu gdb aarch64-linux-gnu-gcc meson ninja
```

> [!TIP]
> Some QEMU packages may be depraceted nad have no support for the raspi4b machine.
> It is recommended to build QEMU from sources.

### Setup Project

```bash
git clone https://github.com/PiVOS-Team/PiVOS.git && cd PiVOS \
./tools/setup.sh
```

`aarch64-linux-gnu.txt` should be modified so that the correct binaries are set.

## Build

### Build

Build the project:
```
./tools/build.sh
```

*VS Code*
- This operation can be performed by `Build project` or `Rebuild project` task

### Change build type

Change the build type:
```
./tools/default_build.sh ( release | debug )
```

*VS Code*
- This operation can be performed by `Change build type` task

## Run

Start the project in QEMU:
```
./tools/start.sh
```

*VS Code*
- `Start QEMU` or `Build and start` tasks
- Notice - `Run->Run Without Debugging` doesn't work

## Debug

Start the project in QEMU and wait for GDB to connect:
```
./tools/start_debug.sh
```

connect with GDB:
```
./tools/connect_debugger.sh
```

*VS Code*
- `Start QEMU in debug` or `Build and start debug` tasks
- Also you can use `Run->Start Debugging`, press `F5`, or use `Run and Debug` menu

> [!TIP]
> `[ No Assembly Available ]` message is normal and is easily fixed by stepping.

## Utils

Format or lint sources:
```
./tools/utils.sh ( clang-format | clang-tidy )
```

*VS Code*
- No special action is required - by default file is formatted every save

## Customize

The following options are available in the `pivos/meson.options` file:
- `gdb_server_address` - address of machine on which QEMU is running
- `gdb_debug_port` - port on which QEMU and GDB will be launched

After making changes
```
./tools/setup.sh --reconfigure
```
