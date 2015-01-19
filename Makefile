NASM := nasm
QEMU := qemu-system-i386
LD := ld
CC := gcc
OBJCOPY := objcopy

ARCH ?= ia32

# generic cflags
CFLAGS += -O2 -fno-strict-aliasing -ffreestanding -fno-stack-protector -fno-stack-check -mno-mmx -mno-sse -nostdinc --std=c11
CFLAGS += -I. -Iinclude/ -Iinclude/lib

# arch-specific cflags
ifeq ($(ARCH), x86_64)
	CFLAGS += mno-red-zone -m64
endif

ifeq ($(ARCH), ia32)
	CFLAGS += -m32
endif

#generic linker flags
LDFLAGS += -nostdlib --warn-common --no-undefined --fatal-warnings 

OBJS := main.o display.o serio.o gdbstub.o arch/init.o arch/dbg.o lib/ctype.o lib/string.o lib/assert.o lib/stdlib.o

all: boot0 kernel

run: floppy.img
	$(QEMU) -hda $< -s -serial stdio

clean:
	rm -rf *.o
	rm -rf lib/*.o
	rm -rf arch/*.o
	rm -rf boot0
	rm -rf main
	rm -rf kernel
	rm -rf floppy.*

boot0: boot0.asm
	$(NASM) -f bin -o $@ $<

kernel: main
	$(OBJCOPY) -j .text -j .data -O binary $< $@

main: $(OBJS)
	$(LD) $(LDFLAGS) -T main.lds -o $@ $^

floppy.img: boot0 kernel
	cat boot0 kernel > floppy.tmp
	dd if=floppy.tmp of=floppy.img bs=512 conv=sync
	
%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o:%.asm
	$(NASM) -f elf32 -o $@ $<
