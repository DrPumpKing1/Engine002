#pragma once
#include <filesystem>

#if defined(__linux__)
    #include <unistd.h>
    #include <limits.h>
#elif defined(_WIN32)
    #include <windows.h>
#elif defined(__APPLE__)
    #include <mach-o/dyld.h>
#endif

std::filesystem::path GetExecutablePath();
std::filesystem::path GetExecutableDir();
std::filesystem::path GetResourcesPath();
