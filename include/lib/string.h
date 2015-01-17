#ifndef NOOS_LIB_STRING_H
#define NOOS_LIB_STRING_H

#include "_defs.h"
#include "stddef.h"

void* NOOS_LIBC memcpy(void* dst, const void* src, unsigned long num);
void* NOOS_LIBC memset(void* ptr, int value, unsigned long num);

size_t NOOS_LIBC strlen(const char* str);

#endif 

