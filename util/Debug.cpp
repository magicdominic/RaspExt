
#include "util/Debug.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

void pi_error2(const char* str, ...)
{
    va_list vl;
    va_start(vl, str);
    vprintf(str, vl);
    va_end(vl);

    exit(1);
}

void pi_message(const char* str, ...)
{
    va_list vl;
    va_start(vl, str);
    vprintf(str, vl);
    va_end(vl);
}
