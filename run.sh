#!/bin/bash

# prepare folder
[ ! -d run ] && mkdir run
pushd run > /dev/null

# download ovmf
if [ ! -f OVMF.fd ]; then
    wget http://downloads.sourceforge.net/project/edk2/OVMF/OVMF-X64-r15214.zip -O ovmf.zip
    unzip ovmf.zip OVMF.fd
    rm -rf ovmf.zip
fi

# create boot file system structure
mkdir -p ./hda/EFI/BOOT
cp ../advisor.efi ./hda/EFI/BOOT/BOOTx64.efi

# run qemu
qemu-system-x86_64 -bios OVMF.fd -L . -hda fat:./hda
popd > /dev/null
