#!/bin/bash

COMPILER="/usr/local/i386elfgcc/bin"

MKDISK=false

while getopts "hm" f; do
    case $f in
        h)
            echo "oprtions:"
            echo "  -m make new virtual_disk"   
            exit 0
        ;;
        
        m)
            MKDISK=true
        ;;
        \?)
        echo "Invalid flag"
        exit 0
        ;;
    esac
done




mkdir -p "./orgs"
mkdir -p "./orgs/lib"
mkdir -p "./orgs/kernel"
mkdir -p "./orgs/stage3"

# Delete old origin files to ensure nothig not supposed to gets liked by the linker
echo "Deleting old orgs..."
for f in ./orgs/kernel/*.o ; do
    rm -f "$f"
done
for f in ./orgs/lib/*.o ; do
    rm -f "$f"
done
for f in ./orgs/stage3/*.o ; do
    rm -f "$f"
done



# Compile standard lib
for in_file in ./lib/*.c ./lib/*.cpp; do 
    if [[ -f "$in_file" ]]; then
        echo "Compiling $in_file"

        basename=$(basename "$in_file")
        out_file="./orgs/lib/${basename}_C.o"

        "$COMPILER/i386-elf-gcc" -ffreestanding -m32 -g -c -Wno-write-strings "$in_file" -o "$out_file"
    fi
done

for in_file in "$dir"/*.asm; do
    if [[ -f "$in_file" ]]; then 
        echo "Compiling $in_file"
        
        basename=$(basename "$in_file" ".asm")

        out_file="./orgs/lib/${basename}_ASM.o"

        nasm "$in_file" -f elf -o "$out_file"

    fi
done

ar rcs libkrnlib.a ./orgs/lib/*.o

#echo "Compiling bootloader..."

#nasm "bootloader/boot.asm" -f bin -o "stage_1.bin"
#nasm "bootloader/stage2/stage_2.asm" -f bin -o "stage_2.bin"

#for in_file in bootloader/stage3/*.asm; do
#    if [[ -f "$in_file" ]]; then
#        echo "Compiling $in_file"

#        basename=$(basename "$in_file" ".asm")
#        out_file="orgs/stage3/${basename}_ASM.o"

#        nasm "$in_file" -f elf -o "$out_file"

#    fi
#done

#for in_file in bootloader/stage3/*.c; do
#    if [[ -f "$in_file" ]]; then
#        echo "Compiling $in_file"

#        basename=$(basename "$in_file" ".c")
#        out_file="orgs/stage3/${basename}_C.o"

#        "$COMPILER/i386-elf-gcc" -ffreestanding -m32 -g -c -Wno-write-strings -I./lib "$in_file" -o "$out_file"
#    fi
#done

#stage_3_files="orgs/stage3/stage_3_entry_ASM.o"
#for in_file in orgs/stage3/*.o; do
#    basename=$(basename "$in_file" ".o")

#    if [[ -f "$in_file" ]] && [[ "$basename" != "stage_3_entry_ASM" ]]; then
#        stage_3_files="$stage_3_files $in_file"
#    fi
#done

#"$COMPILER/i386-elf-ld" -Ttext 0x8200 -o "stage_3.bin" $stage_3_files -L. -lkrnlib --oformat binary

#size=$(stat -c%s ./stage_3.bin)
#echo "Stage 3 sectors: $(( (size + 511) / 512 ))"

#cat "stage_1.bin" "stage_2.bin" "stage_3.bin"> "boot.bin"
#rm "stage_1.bin" "stage_2.bin" "stage_3.bin"


#boot_filesize=$(stat -c%s ./boot.bin)
#padding_size=$((512*2048 - $boot_filesize))

#dd if=/dev/zero of=padding.bin bs=1 count=$padding_size
#cat boot.bin padding.bin >> full_boot.bin
#rm boot.bin padding.bin

#mv full_boot.bin boot.bin



# Compile kernel files
for dir in ./kernel ./drivers ./not_kernel; do
    for in_file in "$dir"/*.c "$dir"/*.cpp; do
        if [[ -f "$in_file" ]]; then
            echo "Compiling: $in_file"

            basename=$(basename "$in_file")

            out_file="./orgs/kernel/${basename}_C.o"

            "$COMPILER/i386-elf-gcc" -ffreestanding -m32 -g -c -Wno-write-strings -I./lib "$in_file" -o "$out_file" 
        fi
    done

    for in_file in "$dir"/*.asm; do
        if [[ -f "$in_file" ]]; then
            echo "Compiling: $in_file"

            basename=$(basename "$in_file" ".asm")

            out_file="./orgs/kernel/${basename}_ASM.o"

            nasm "$in_file" -f elf32 -o "$out_file"
        fi
    done
done

# Chaining all file names in the same string to pass them as an argument to the linker
kernel_files="./orgs/kernel/kernel_entry_ASM.o"
for in_file in ./orgs/kernel/*.o; do
    basename=$(basename "$in_file" ".o")
    if [[ -f "$in_file" ]] && [[ "$basename" != "kernel_entry_ASM" ]]; then
        kernel_files="$kernel_files $in_file"
    fi
done

echo "Linking kernel..."

#"$COMPILER/i386-elf-ld" -T "linker.ld" -Ttext 0x100000 -o "full_kernel.elf" $kernel_files -L. -lkrnlib
#"$COMPILER/i386-elf-ld" -T "linker.ld"  -Ttext 0x100000 -o "full_kernel.o" $kernel_files -L. -lkrnlib
#"$COMPILER/i386-elf-ld" -T "linker.ld"  -Ttext 0x100000 -o "full_kernel.bin" $kernel_files -L. -lkrnlib --oformat binary
"$COMPILER/i386-elf-ld" -T "linker.ld" -o "full_kernel.elf" $kernel_files -L. -lkrnlib
"$COMPILER/i386-elf-ld" -T "linker.ld" -o "full_kernel.o" $kernel_files -L. -lkrnlib
"$COMPILER/i386-elf-ld" -T "linker.ld" -o "full_kernel.bin" $kernel_files -L. -lkrnlib --oformat binary
objdump -d ./full_kernel.o > dump.txt

if [[ "$MKDISK" == true ]]; then
    echo "Creating virtual file system"

    rm -f virtual_disk.img
    dd if=/dev/zero of=virtual_disk.img bs=1M count=1024

    sudo parted virtual_disk.img --script mklabel msdos
    sudo parted virtual_disk.img --script mkpart primary fat32 1MiB 100%

    sudo modprobe loop

    LOOPDEV=$(sudo losetup -f --show -P virtual_disk.img)
    echo "Loop device: $LOOPDEV"
    sleep 1

    ls -la ${LOOPDEV}*

    PARTDEV="${LOOPDEV}p1"
    echo "Partition device: $PARTDEV"

    sudo mkfs.vfat -F 32 "$PARTDEV"

    mkdir -p mnt
    sudo mount "$PARTDEV" mnt

    sudo mkdir -p mnt/boot/grub
    sudo cp grub/grub.cfg mnt/boot/grub
    sudo cp full_kernel.elf mnt/boot/kernel.elf

    echo "Installing grub"
    sudo grub-install \
        --target=i386-pc \
        --boot-directory=mnt/boot \
        --modules="part_msdos fat multiboot" \
        "$LOOPDEV"

    sudo umount mnt
    sudo losetup -d "$LOOPDEV"

else
    sudo modprobe loop

    LOOPDEV=$(sudo losetup -f --show -P virtual_disk.img)
    echo "Loop device: $LOOPDEV"
    sleep 1

    ls -la ${LOOPDEV}*

    PARTDEV="${LOOPDEV}p1"
    echo "Partition device: $PARTDEV"

    sudo mount "$PARTDEV" mnt
    sudo cp full_kernel.elf mnt/boot/kernel.elf

    sudo umount mnt
    sudo losetup -d "$LOOPDEV"
fi


#cat "boot.bin" "full_kernel.bin" > "everything.bin"
#cat "everything.bin" "zeros.bin" > "OS.bin"
#rm "boot.bin" "full_kernel.bin" "everything.bin" "full_kernel.o"

#dd if=/dev/zero of=OS.iso bs=1M count=10
#dd if=OS.bin of=OS.iso conv=notrunc





echo "Starting virtual machine"
#qemu-system-x86_64 -drive format=raw,file="OS.bin",index=0,if=floppy -m 128M
#qemu-system-x86_64 -no-reboot -d int,cpu_reset -D qemu.log -drive format=raw,file="OS.iso",index=0,if=ide -m 2G
qemu-system-x86_64 -no-reboot -d int,cpu_reset -D qemu.log -drive format=raw,file="virtual_disk.img",index=0,if=ide -m 2G -serial stdio
#qemu-system-x86_64 -cdrom OS.iso -m 128M
