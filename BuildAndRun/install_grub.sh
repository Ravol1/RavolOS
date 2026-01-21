#!/bin/bash

readonly NAME=$(basename "$0")
usage() {
    cat << EOF
Usage: $NAME -i IMAGE [OPTIONS]

Description:
    Installs grub in the specfied virtual disk image
Options:
    -i IMAGE    Path to the image file.
    -c          Specify a grub confing file (default: uses ./grub/grub.cfg).
    -h          Display this information.
EOF
}



CFG="./grub/grub.cfg"

while getopts "hi:c:" f; do
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

        c)
            CFG="$OPTARG"

            if [ -z "$CFG" ]; then
                echo "error: missing argument for -i"
                exit 1
            fi
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

echo "Installing grub"
LOOPDEV=$(sudo losetup -f --show -P "$IMG")
echo "Loop device: $LOOPDEV"
sleep 1

ls -la ${LOOPDEV}*

PARTDEV="${LOOPDEV}p1"
echo "Partition device: $PARTDEV"

mkdir -p ./mnt
sudo mount "$PARTDEV" ./mnt

sudo mkdir -p ./mnt/boot/grub
sudo cp "$CFG" ./mnt/boot/grub


sudo grub-install \
    --target=i386-pc \
    --boot-directory=mnt/boot \
    "$LOOPDEV"

sudo umount mnt
sudo losetup -d "$LOOPDEV"