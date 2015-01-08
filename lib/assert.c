#include <assert.h>

void _assert(const char* file, long line, const char* exp)
{
    //fprintf(stderr, "Assertion failed: %s, file %s, line %l\n", exp, file, line);
    __asm__("int $3");
}

