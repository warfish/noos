OUTPUT_FORMAT("elf64-x86-64", "elf64-x86-64", "elf64-x86-64")
OUTPUT_ARCH(i386:x86-64)
ENTRY(noos_start)

SECTIONS
{
    . = 0x0;
    __image_base = .;
    
    .text : 
    {
        *(.noos_start)  /* Must come first */
        *(.text)
    }
    . = ALIGN(4096);

    .data :
    {
        *(.data)
        *(.rodata*)
        *(.bss)
    }
    . = ALIGN(4096);

    /DISCARD/ :
    {
        *(.eh_frame)
        *(.comment)
    }
}

