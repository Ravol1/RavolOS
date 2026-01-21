#!/bin/bash


MKDISK=false

while getopts "m" f; do
    case $f in    
        m)
            MKDISK=true
        ;;
        \?)
        echo "Invalid flag"
        exit 0
        ;;
    esac
done

SCRIPTS="./BuildAndRun"
KER="./build/kernel.bin"
IMG="./build/hdd.img"

"$SCRIPTS"/build.sh -o "$KER"


if [[ "$MKDISK" == true ]]; then
    "$SCRIPTS"/disk_setup.sh -i "$IMG" -k "$KER" -m
    "$SCRIPTS"/install_grub.sh -i "$IMG"
else
    "$SCRIPTS"/disk_setup.sh -i "$IMG" -k "$KER"
fi

"$SCRIPTS"/run_qemu.sh -i "$IMG" -l "qemu.log"
objdump -d "$KER" > kernel_dump.asm




