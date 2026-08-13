#pragma once

#if defined(DEBUG)
    #include <cstdio>

    void LogImpl(const char* fmt, ...);

    #define LOG(...) LogImpl(__VA_ARGS__)
#else
    #define LOG(...) do {} while(0)
#endif
