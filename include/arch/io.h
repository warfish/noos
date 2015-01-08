#ifndef NOOS_ARCH_IO_H
#define NOOS_ARCH_IO_H

static inline void outb(uint16_t port, uint8_t byte)
{
    __asm__("out %%al, %%dx"
            :
            :"d"(port), "a"(byte)
            :);
}

static inline uint8_t inb(uint16_t port)
{
    uint8_t byte = 0;

    __asm__("in %%dx, %%al"
            :"=a"(byte)
            :"d"(port)
            :);

    return byte;
}

#endif

