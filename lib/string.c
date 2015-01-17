#include <string.h>
#include <stdlib.h>


void* memcpy(void* dst, const void* src, unsigned long num)
{
    if (dst && src && num)
    {
        char* dst8 = (char*) dst;
        const char* src8 = (const char*) src;

        while (num--)
        {
            *dst8++ = *src8++;
        }
    }

    return dst;
}

void* memset(void* ptr, int value, unsigned long num)
{
    if (ptr)
    {
        char val8 = (char)value;
        char* ptr8 = (char*) ptr;
        while(num-- > 0)
        {
            *ptr8++ = val8;
        }
    }

    return ptr;
}

size_t strlen(const char* str)
{
    if (str == NULL) {
        return 0;
    }

    size_t len = 0;
    while (*str++ != '\0') {
        ++len;
    }

    return len;
}

