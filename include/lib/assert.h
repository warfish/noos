#ifndef NOOS_LIB_ASSERT_H
#define NOOS_LIB_ASSERT_H

#include "_defs.h"

#ifdef NDEBUG
#   define assert(_exp_) 
#else
    void NOOS_LIBC _assert(const char* file, long line, const char* exp);
#   define assert(_exp_)    do { if((int)(_exp_) == 0) _assert(__FILE__, __LINE__, #_exp_); } while(0);
#endif

#endif

