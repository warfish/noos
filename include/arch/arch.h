#ifndef _ARCH_ARCH_H
#define _ARCH_ARCH_H

struct cpu_context 
{
    uint32_t dr0;
    uint32_t dr1;
    uint32_t dr2;
    uint32_t dr3;
    uint32_t dr6;
    uint32_t dr7;
    uint16_t gs;
    uint16_t fs;
    uint16_t es;
    uint16_t ss;
    uint16_t ds;
    uint16_t cs;
    uint32_t eflags;
    uint32_t eip;
    uint32_t esp;
    uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
} __attribute__((packed));

void arch_init(void);

void arch_disable_interrupts();
void arch_enable_interrupts();

int arch_save_disable_interrupts();
void arch_restore_interrupts(int state);

typedef void(*arch_interrupt_handler_t)(void);
void arch_install_interrupt_handler(int interrupt, arch_interrupt_handler_t handler);

#endif

