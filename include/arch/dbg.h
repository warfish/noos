#ifndef _ARCH_DBG_H
#define _ARCH_DBG_H

struct cpu_context;

typedef void(*dbg_exception_handler_t)(int vector, struct cpu_context* ctx);
void dbg_install_exception_handler(dbg_exception_handler_t handler);

#endif

