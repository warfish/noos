#include <stdint.h>
#include <assert.h>

#include <noos/serio.h>
#include <arch/io.h>

void serio_init(uint16_t iobase, int ratediv)
{
    // On magic numbers see here: http://geezer.osdevbrasil.net/osd/serial/index.htm
    outb(iobase + SIO_INTR_OFF,     0);                         // Disable all interrupts
    outb(iobase + SIO_LCR_OFF,      0x80);                      // Divisor latch access to set baud rate divisor
    outb(iobase + SIO_BAUD_LOW_OFF, ratediv & 0xFF);            // Divisor low byte
    outb(iobase + SIO_BAUD_HI_OFF,  (ratediv >> 8) & 0xFF);     // Divisor high byte
    outb(iobase + SIO_LCR_OFF,      0x03);                      // Parity none, 7 data bits, 1 stop bit
    outb(iobase + SIO_FCR_OFF,      0xC7);                      // Tx/Rx FIFO
    outb(iobase + SIO_MCR_OFF,      0x0B);                      // DTR, RTS
    
    // Scratch reg can be used for any purpose - we will store initialized flag there
    outb(iobase + SIO_SCR_OFF,      1);                         
    assert(inb(iobase + SIO_SCR_OFF) == 1);
}

void serio_write(uint16_t iobase, uint8_t byte)
{
    assert(inb(iobase + SIO_SCR_OFF) == 1);
    while(!(inb(iobase + SIO_LSR_OFF) & SIO_MSR_TR_READY))
        ;

    outb(iobase + SIO_DATA_OFF, byte);
}

uint8_t serio_read(uint16_t iobase)
{
    assert(inb(iobase + SIO_SCR_OFF) == 1);
    while(!(inb(iobase + SIO_LSR_OFF) & SIO_MSR_DATA_READY))
        ;

    return inb(iobase + SIO_DATA_OFF);
}

int serio_poll(uint16_t iobase)
{
    return inb(iobase + SIO_LSR_OFF) & SIO_MSR_DATA_READY;
}

