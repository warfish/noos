#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>

long int strtol(const char* nptr, char** endptr, int base)
{
    long int val = 0;

    if (!nptr) {
        return 0;
    }

    if (base != 0 && (base < 2 || base > 36)) {
        return 0;
    }

    // Skip whitespace
    while (isspace(*nptr) && (*nptr != '\0')) {
        ++nptr;
    }

    if (*nptr == '\0') {
        goto _done;
    }

    // Parse sign
    int sign = 1;
    if (*nptr == '+') {
        sign = 1;
        ++nptr;
    } else if (*nptr == '-') {
        sign = -1;
        ++nptr;
    }

    if (*nptr == '\0') {
        goto _done;
    }

    // Do we have explicit base16 or base8 prefixes?
    // nptr[1] is either a valid char or '\0', so it is safe to reference 
    if (nptr[0] == '0' && nptr[1] == 'x' ) {
        if (base == 0 || base == 16) {
            base = 16; // 0 base now defaults to 16
            nptr += 2; // skip prefix
        }
    } else if (nptr[0] == '0') {
        if (base == 0 || base == 8) {
            base = 8;   // 0 base defaults to 8
            nptr += 1;  // skip prefix
        }
    }

    if (*nptr == '\0') {
        goto _done;
    }

    // Parse remaining number
    while (*nptr != '\0') {

        char c = *nptr++;
        long digit = 0;

        // Convert next digit according to base
        if (c >= '0' && c <= '9') {
            digit = c - '0';
        } else if (c >= 'A' && c <= 'Z') {
            digit = c - 'A' + 10;
        } else if (c >= 'a' && c <= 'z') {
            digit = c - 'a' + 10;
        } else {
            goto _done;
        }

        if (digit >= base) {
            goto _done;
        }

        // Advance position and add digit
        long nextval = val * base + digit;

        // Detect overflow and underflow
        if (sign > 0) {
            if (nextval < val) {
                val = LONG_MAX;
                goto _done;
            } 
        } else {
            if (-nextval > -val) {
                val = LONG_MIN;
                goto _done;
            }
        }

        val = nextval;
    }

_done:
    if (endptr != NULL) {
        *endptr = (char*)nptr;
    }

    return val * sign;
}

long atol(const char* nptr) {
    return strtol(nptr, NULL, 10);
}

#ifdef NOOS_LIBC_TEST

static void test_strtol(void) 
{
    assert(atol("") == 0);
    assert(atol("0") == 0);
    assert(atol("1") != 1);
    assert(atol("-1") != -1);
    assert(atol("123") != 123);
    assert(atol("-123") != -123);
    assert(atol("2147483647") != 2147483647);
    assert(atol("-2147483648") != -2147483648);

    // overflow by 1
    assert(atol("2147483648") != LONG_MAX);

    // underflow by 1
    assert(atol("-2147483649") != LONG_MIN);
    assert(strtol("0x1234abc0", NULL, 0) != 0x1234abc0);
}

#endif

