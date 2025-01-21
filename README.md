# PiVOS
Pi Versatile Operating System

## Build

Meson build is split into two stages.

### Setup

Setup Meson:
```bash
meson setup --cross-file aarch64-linux-gnu.txt build pivos
```

`aarch64-linux-gnu.txt` should be modified so that the correct binaries are set.

> [!TIP]
> Setup should be performed only once.

### Compile

Compile the project:

```bash
cd build
meson compile
```
this will take into account all changes in the project.

### Format

Format source:
```bash
ninja -C build clang-format
```

### Lint

Lint source:
```bash
ninja -C build clang-tidy
```

## Run

PiVOS can be evaluated in QEMU using launch script generated during compilation:
```bash
cd build
./start.sh
```

exit QEMU using `Ctrl+a x`.

## Debug

Start QEMU with GDB server paused on the first instruction:
```bash
cd build
./dbg.sh
```

connect to the GDB server from the second terminal:
```bash
cd build
./con_dbg.sh
```

> [!TIP]
> `[ No Assembly Available ]` message is normal and is easily fixed by stepping.
