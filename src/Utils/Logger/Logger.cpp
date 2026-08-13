#include "Logger.h"

#if defined(DEBUG)
    #include <cstdarg>
    #include <cstdio>

    void LogImpl(const char *fmt, ...) {
        va_list args;
        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);
        printf("\n");
    }
#endif
