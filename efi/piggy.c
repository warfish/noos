#include <efi.h>
#include <efilib.h>

// Generated by objcopy to point to embedded kernel binary
extern uint8_t _binary____kernel_start[];
extern uint8_t _binary____kernel_end[];

EFI_STATUS efi_main(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE* systemTable)
{
    InitializeLib(imageHandle, systemTable);

    EFI_STATUS status = EFI_SUCCESS;
    EFI_LOADED_IMAGE* loadedImage = NULL;

    status = systemTable->BootServices->HandleProtocol(imageHandle, &LoadedImageProtocol, (void**)&loadedImage);
    if (EFI_ERROR(status)) {
        Print(L"No loaded image\n");
        return status;
    }

    uint8_t* p = _binary____kernel_start;

    Print(L"piggy.efi loaded @ 0x%x\n", loadedImage->ImageBase);
    Print(L"Binary kernel @ 0x%x - 0x%x, %d\n", _binary____kernel_start, _binary____kernel_end, (_binary____kernel_end - _binary____kernel_start));

    if (p[1] != 'E' || p[2] != 'L' || p[3] != 'F') {
        Print(L"> %c%c%c\n", p[1], p[2], p[3]);
        Print(L"Not an ELF image\n");
        return EFI_UNSUPPORTED;
    }

    // NEXT: parse and relocate embedded ELF image
    
    return EFI_SUCCESS;
}

