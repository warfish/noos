#ifndef NOOS_LIB_STDLIB_H
#define NOOS_LIB_STDLIB_H

#include "_defs.h"

#define NULL 0

typedef unsigned long size_t;

long strtol(const char* nptr, char** endptr, int base);
long atol(const char* nptr);

#endif
