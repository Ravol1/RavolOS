#!/bin/bash

mkdir -p ~/tmp
export TMPDIR=~/tmp

sudo pacman -S base-devel git wget bison flex gmp libmpc mpfr isl texinfo
mkdir -p ~/cross/i386-elf
cd ~/cross/i386-elf

wget https://ftp.gnu.org/gnu/binutils/binutils-2.42.tar.gz
tar -xvf binutils-2.42.tar.gz

wget https://ftp.gnu.org/gnu/gcc/gcc-13.2.0/gcc-13.2.0.tar.gz
tar -xvf gcc-13.2.0.tar.gz

mkdir build-binutils
cd build-binutils
../binutils-2.42/configure --target=i386-elf --prefix=/usr/local/cross --disable-nls --disable-werror
make -j$(nproc)
sudo make install
cd ..

mkdir build-gcc
cd build-gcc
../gcc-13.2.0/configure --target=i386-elf --prefix=/usr/local/cross --disable-nls --enable-languages=c --without-headers
make all-gcc -j$(nproc)
sudo make install-gcc
cd ..

echo 'export PATH=/usr/local/cross/bin:$PATH' >> ~/.bashrc
source ~/.bashrc
