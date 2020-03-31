#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

void slog(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    // return r;
}