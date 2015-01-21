#ifndef NOOS_LIB__DEFS_H
#define NOOS_LIB__DEFS_H

#ifdef __x86_64__
#   define NOOS_LIBC
#else
#   define NOOS_LIBC __attribute__((cdecl))
#endif

#endif

