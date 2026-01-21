#!/bin/bash


readonly NAME=$(basename "$0")
usage() {
    cat << EOF
Usage: $NAME -o PATH

Description:
  Compiles kernel in a binary file

Options:
    -o FILE     Specify where to save kernel binary file.
    -h          Display this information."
EOF
}


while getopts "ho:" f; do
    case $f in
        h)
            usage
            exit 0
        ;;
        
        o)
            OUT="$OPTARG"
            if [ -z "$OUT" ]; then
                echo "error: missing argument for -o"
                exit 1
            fi
        ;;
        \?)
            exit 1
        ;;
    esac
done

if [ -z "$OUT" ]; then
    echo "error: missing option -o"
    exit 1
fi

make clean
make KERNEL_BIN="$OUT"
