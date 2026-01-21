#!/bin/bash

readonly NAME=$(basename "$0")
usage() {
    cat << EOF
Usage: $NAME -i IMAGE [OPTIONS]

Description:
    Installs grub in the specfied virtual disk image
Options:
    -i IMAGE        Specify path to the image file.
    -l LOG FILE     Specify path to qemu log file
    -h              Display this information.
EOF
}


while getopts "hi:l:" f; do
    case "$f" in
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

        l)
            LOG="$OPTARG"
            if [ -z "$LOG" ]; then
                echo "error: missing argument for -l"
                exit 1
            fi
        ;;

        \?)
            exit 1
        ;;
    esac
done

if [ -z "$IMG" ]; then
    echo "error: missing option -i"
    exit 1
fi

if [ -z "$LOG" ]; then
    qemu-system-x86_64 -no-reboot -drive format=raw,file="$IMG",index=0,if=ide -m 2G -serial stdio

else
    qemu-system-x86_64 -no-reboot -d int,cpu_reset -D "$LOG" -drive format=raw,file="$IMG",index=0,if=ide -m 2G -serial stdio
fi



