# RavolOS

RavolOS is a hobby operating system designed to learn and experiment with low-level programming, built from scratch with a custom kernel and bootloader support.

## Features

- Multiboot2 support 
- Makefile-based build system
- Automated build and run scripts

## Requirements
Before building RavolOS, ensure you have the following dependencies installed:

- GCC cross-compiler for i386 (if not installed globally, you'll need to adjust the Makefile manually)
- NASM
- Mtools
- GRUB 2 - If you want to boot using grub
- QEMU - If you want to run using the included script

## Building and running

# Quick Start
For a quick build and run with default settings:
```bash
./start.sh -m
```
To use an existing disk image:
```bash
./start.sh
```

# Manual Build Process
RavolOS uses a modular script-based build system. For detailed instructions, see BUILD.md.

1. Compile the kernel:
```bash 
./BuildAndRun/build.sh -o kernel.bin
```
2. Set up the disk image:
```bash
./BuildAndRun/disk_setup.sh -i disk.img -k kernel.bin
```
3. Install GRUB:
```bash
./BuildAndRun/install_grub.sh -i disk.img
```
4. Run with QEMU:
```bash  
./BuildAndRun/run_qemu.sh -i disk.img
```

Alternatively, use make to compile the kernel.

## License

Released under the [MIT License](LICENSE).  
More on the license can be found in the `LICENSE` file.
