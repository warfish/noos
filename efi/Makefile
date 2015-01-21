#
# This makefile builds noos EFI piggyback loader
#

CC = gcc
LD = ld
OBJCOPY = objcopy

GNUEFI_INC ?= /usr/include/efi
GNUEFI_BIN ?= /usr/lib

ARCH ?= $(shell $(CC) -dumpmachine | cut -f1 -d- | sed s,i[3456789]86,ia32,)

# Arch-specific compilation flags
CPPFLAGS += -DCONFIG_$(ARCH)
ifeq ($(ARCH),ia64)
  CFLAGS += -mfixed-range=f32-f127
endif

ifeq ($(ARCH),ia32)
  CFLAGS += -mno-mmx -mno-sse
endif

ifeq ($(ARCH),x86_64)
  GCCVERSION   := $(shell $(CC) -dumpversion | cut -f1 -d.)
  GCCMINOR     := $(shell $(CC) -dumpversion | cut -f2 -d.)

  # Rely on GCC MS ABI support?
  GCCNEWENOUGH := $(shell ( [ $(GCCVERSION) -gt "4" ]           \
                            || ( [ $(GCCVERSION) -eq "4" ]      \
                                 && [ $(GCCMINOR) -ge "7" ] ) ) \
                          && echo 1)
  ifeq ($(GCCNEWENOUGH),1)
    CPPFLAGS += -DGNU_EFI_USE_MS_ABI -maccumulate-outgoing-args --std=c11
  endif

  CFLAGS += -mno-red-zone -mno-mmx -mno-sse
endif

CFLAGS += -O2 -fpic -fshort-wchar -fno-strict-aliasing -fno-merge-constants -ffreestanding -fno-stack-protector -fno-stack-check
CFLAGS += -I. -I$(GNUEFI_INC) -I$(GNUEFI_INC)/$(ARCH) -I $(GNUEFI_INC)/protocol

LDFLAGS += -nostdlib --warn-common --no-undefined --fatal-warnings -shared -Bsymbolic 
LDFLAGS += -L$(GNUEFI_BIN) $(GNUEFI_BIN)/crt0-efi-$(ARCH).o -T $(GNUEFI_BIN)/elf_$(ARCH)_efi.lds

LIBS += -lefi -lgnuefi

FORMAT := --target efi-app-$(ARCH)

OBJS := piggy.o

TARGET := piggy.efi

#
#
#

all: $(TARGET)

clean:
	rm -rf *.efi
	rm -rf *.so
	rm -rf *.o
	rm -rf efi/*.o

%.efi: %.so
	$(OBJCOPY) -j .text -j .sdata -j .data -j .dynamic -j .dynsym -j .rel* -j .rela* -j .reloc $(FORMAT) $*.so $@

%.so: $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $@ $(LIBS)
	
%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

