## Building RavolOS
This guide provides detailed instructions for building and running RavolOS.

# Prerequisites
Ensure all required dependencies are installed on your system:

- GCC cross-compiler for i386 architecture

If not installed globally, you'll need to manually adjust the Makefile to point to your cross-compiler

- NASM
- Mtools
- GRUB 2 for bootloader installation
- QEMU for emulation


# Build Scripts
RavolOS provides several scripts in the BuildAndRun/ directory to automate the build process.

1. Building the Kernel
Compile the kernel source files into a binary:
```bash
./BuildAndRun/build.sh -o <OUTPUT_PATH>
```

Options:
- -o <PATH> - Specifies the output path for the compiled kernel binary

2. Setting Up the Disk Image
Copy the kernel binary to a disk image file:
bash./BuildAndRun/disk_setup.sh -i <IMAGE> -k <KERNEL> [OPTIONS]
Options:
- -i <IMAGE> - Path to the disk image file
- -k <KERNEL> - Path to the kernel binary
- -m - Create a new disk image with the specified name, partition it, and copy the kernel

Examples:
```bash
# Copy kernel to existing image
./BuildAndRun/disk_setup.sh -i disk.img -k kernel.bin

# Create new image and copy kernel
./BuildAndRun/disk_setup.sh -i disk.img -k kernel.bin -m
```

3. Installing GRUB
Install GRUB bootloader on the disk image:
```bash
./BuildAndRun/install_grub.sh -i <IMAGE> [OPTIONS]
```
Options:
- -i <IMAGE> - Path to the disk image file
- -c <CONFIG> - Use a custom GRUB configuration file (default: ./grub/grub.cfg)

Examples:
```bash
# Install with default configuration
./BuildAndRun/install_grub.sh -i disk.img

# Install with custom configuration
./BuildAndRun/install_grub.sh -i disk.img -c /path/to/custom/grub.cfg
```

4. Running with QEMU
Launch QEMU with the disk image:
```bash
./BuildAndRun/run_qemu.sh -i <IMAGE> [OPTIONS]
```
Options:
- -i <IMAGE> - Path to the disk image file
- -l <LOG_PATH> - Specify a path for QEMU log output (if omitted, QEMU runs without logging)

# Quick Build with start.sh
The start.sh script automates the entire build process with default options

```bash
./start.sh [OPTIONS]
```
Options:
-m - Create a new disk image before building

What it does:

Compiles the kernel
Sets up the disk image
Installs GRUB with default configuration
Runs QEMU with loging
Dumps the kernel binary

# Default Output Locations
When using start.sh or the build scripts with default settings, the following files are generated:

|     File      |       Location      |
| ------------- | ------------------- |
| Kerenl binary | ./build/kernel.bin  |
|  Disk image   |   ./build/hdd.img   |
|   QEMU log    |     ./quemu.log     |
|  Kernel dump  |  ./kernel_dump.asm  |

# Building with Make
You can also compile the kernel directly using the Makefile:
```bash
make
```
