#!/bin/bash

MKDISK=false


readonly NAME=$(basename "$0")
usage() {
    cat << EOF
Usage: $NAME -i IMAGE -k KERNEL [OPTIONS]

Description:
  Creates and/or copies kernel binary file to a disk image

Options:
    -i IMAGE    Path to the image file.
    -k KERNEL   Path to the kernel binary file.
    -m          Make new image file (default: use existing file).
    -h          Display this information.
EOF
}


while getopts "hmi:k:l:" f; do
    case $f in
        h)
            usage           
            exit 0
        ;;
        i)
            IMG="$OPTARG"
            if [ -z "$IMG" ]; then
                echo "error: missing argument for -i"
                exit 1
            fi
        ;;
        k)
            KER="$OPTARG"
            if [ -z "$KER" ]; then
                echo "error: missing argument for -i"
                exit 1
            fi
        ;;
        m)
            MKDISK=true
        ;;
        
        \?)
        exit 1
        ;;
    esac
done

if [ -z "$IMG" ]; then
    echo "error: missing argument for -i"
    exit 1
fi
if [ -z "$KER" ]; then
    echo "error: missing argument for -k"
    exit 1
fi


if [[ "$MKDISK" == true ]]; then
    echo "Creating virtual file system"

    rm -f "$IMG"
    dd if=/dev/zero of="$IMG" bs=1M count=1024

    parted "$IMG" --script mklabel msdos
    parted "$IMG" --script mkpart primary fat32 1MiB 100%
    
    mkfs.vfat -F 32 --offset 2048 "$IMG"
    mmd -i "$IMG"@@1M ::/boot
fi
mcopy -i "$IMG"@@1M "$KER" ::/boot/kernel.bin -o 