PREFIX = i386-elf-
CC = $(PREFIX)gcc
AS = nasm
LD = $(PREFIX)ld
AR = ar

KERNEL_BIN ?= kernel.bin

BUILD_DIR = obj

<<<<<<< HEAD
CFLAGS  = -m32 -ffreestanding -g -c -Wno-write-strings -Isrc/kernel -Isrc/lib
=======
CFLAGS  = -m32 -ffreestanding -m32 -g -c -Wno-write-strings -Isrc/kernel -Isrc/lib
>>>>>>> refs/remotes/origin/main
ASFLAGS = -f elf32
LDFLAGS = -m elf_i386 -T linker.ld 

K_C_SOURCES = $(shell find src/kernel -type f -name "*.c")
K_ASM_SOURCES = $(shell find src/kernel -type f -name "*.asm")
K_OBJS = $(sort $(K_C_SOURCES:src/%.c=$(BUILD_DIR)/%.o) $(K_ASM_SOURCES:src/%.asm=$(BUILD_DIR)/%_asm.o))

LIB_C_SRCS = $(shell find src/lib -name "*.c")
LIB_OBJS = $(LIB_C_SRCS:src/lib/%.c=$(BUILD_DIR)/lib/%.o)

all: $(KERNEL_BIN)

libkrnlib.a: $(LIB_OBJS)
	$(AR) rcs $@ $(LIB_OBJS)

$(KERNEL_BIN): $(K_OBJS) libkrnlib.a
	@mkdir -p $(dir $(KERNEL_BIN))
	$(LD) $(LDFLAGS) -o $@ $(K_OBJS) -L. -lkrnlib

$(BUILD_DIR)/%.o: src/%.c
	@mkdir -p $(dir $@) 
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/%_asm.o: src/%.asm
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(KERNEL_BIN) libkrnlib.a