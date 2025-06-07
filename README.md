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
./tools/setup_project.sh
```

`aarch64-linux-gnu.txt` should be modified so that the correct binaries are set.

## Build

### Build

*Command*
```
./tools/build.sh [--debug | --release] [--clean] 
```

*Description*
- If no build type is specified the default option will be used
- Running this with `--clean` removes all compilation data 

*VS Code*
- This operation can be performed by `Build project` or `Rebuild project` task

### Default Build

*Command*
```
./tools/default_build.sh (--set | --get ) (--debug | --release)
```

*Description*
- Allows you to change or read the default build type

*VS Code*
- This operation can be performed by `Change build type` task

## Run

*Command*
```
./tools/run.sh [--debug | --release] [--gdb]
```

*Description*
- If no build type is specified the default option will be used
- `--gdb` param can be only used for debug build
- starts QEMU for specified build

*VS Code*
- For both builds 
    - `Start QEMU` or `Build and start` tasks
    - Notice - `Run->Run Without Debugging` doesn't work
- For debug build only (enables GDB) 
    - `Start QEMU in debug` or `Build and start debug` tasks
    - Also you can use `Run->Start Debugging`, press `F5`, or use `Run and Debug` menu

## Debug

*Command*
```
./tools/debugger.sh
```

*Description*
- open gdb with all necessary configuration
- QEMU must be already running (with `--gdb` flag)

*VS Code*
- Described in `Run` section

> [!TIP]
> `[ No Assembly Available ]` message is normal and is easily fixed by stepping.

## Utils

*Command*
```
./tools/utils.sh [--debug | --release] (--format | --tidy)
```

*Description*
- If no build type is specified the default option will be used
- Command use rules specified in `.clang-format` and `.clang-tidy` files

*VS Code*
- No special action is required - by default file is formatted every save

## Customize

The following options are available in the `pivos/meson.options` file:
- `gdb_server_address` - address of machine on which QEMU is running
- `gdb_debug_port` - port on which QEMU and GDB will be launched

After making changes
```
./tools/setup_project.sh --reconfigure
```
