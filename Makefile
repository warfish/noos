NASM := nasm
QEMU := qemu-system-i386
LD := ld
CC := gcc
OBJCOPY := objcopy

ARCH ?= x86_64

# generic cflags
CFLAGS += -O2 -fno-strict-aliasing -ffreestanding -fno-stack-protector -fno-stack-check -mno-mmx -mno-sse -nostdinc --std=c11

# arch-specific cflags
ifeq ($(ARCH), x86_64)
	CFLAGS += -mno-red-zone -m64
endif

ifeq ($(ARCH), ia32)
	CFLAGS += -m32
endif

CFLAGS += -I. -Iinclude/ -Iinclude/lib

#generic linker flags
LDFLAGS += -nostdlib --warn-common --no-undefined --fatal-warnings 

OBJS := main.o display.o serio.o lib/ctype.o lib/string.o lib/assert.o lib/stdlib.o

all: kernel piggy

piggy:
	$(MAKE) -C efi

kernel: $(OBJS)
	$(LD) $(LDFLAGS) -T main.lds -o $@ $^

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o:%.asm
	$(NASM) -f elf32 -o $@ $<

clean:
	rm -rf *.o
	rm -rf lib/*.o
	rm -rf arch/*.o
	rm -rf kernel
	$(MAKE) -C efi clean

.PHONY:piggy

