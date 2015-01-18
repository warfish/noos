#ifndef NOOS_ARCH_CPU_H
#define NOOS_ARCH_CPU_H

struct cpu_context 
{
    uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;
    uint32_t edi;
    uint32_t esi;
    uint32_t esp;
    uint32_t ebp;
    uint32_t eip;
    uint32_t eflags;
    uint16_t cs;
    uint16_t ds;
    uint16_t es;
    uint16_t ss;
    uint16_t fs;
    uint16_t gs;
    uint32_t dr0;
    uint32_t dr2;
    uint32_t dr3;
    uint32_t dr4;
    uint32_t dr7;
};

#endif

