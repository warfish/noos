OUTPUT_FORMAT("elf32-i386", "elf32-i386", "elf32-i386")
OUTPUT_ARCH(i386)
ENTRY(noos_start)

SECTIONS
{
    . = 0x10000;
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

