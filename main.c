//
// Protected mode entry
//
// GDT set to flat 32 bit
// IDT not set, interrupts are disabled
// Video mode 03h, 80x25, 16 colors, 0xB8000

#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>

#include <noos/display.h>
#include <noos/serio.h>

#include <arch/init.h>

// Points to image base
extern void* __image_base;

void noos_start(void) __attribute__((section(".noos_start")));
void noos_start(void)
{
    arch_init();
    display_clear();
    serio_init(SIO_COM1, SIO_B115200);

    __asm__("int $3");
    while (1)
        ;
}

