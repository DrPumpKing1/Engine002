#include "ExecutablePath.h"

std::filesystem::path GetExecutablePath() {
#if defined(__linux__)
    char result[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
    return std::filesystem::path(std::string(result, count > 0 ? count : 0));
#elif defined(_WIN32)
    wchar_t result[MAX_PATH];
    DWORD count = GetModuleFileNameW(NULL, result, MAX_PATH);
    return std::filesystem::path(std::wstring(result, count));
#elif defined(__APPLE__)
    char result[PATH_MAX];
    uint32_t size = sizeof(result);
    if(_NSGetExecutablePath(result, &size) == 0) {
        return std::filesystem::canonical(std::filesystem::path(result));
    }
    return {};
#else
    static_assert(false, "Platform not supported for GetExecutablePath()");
#endif
}

std::filesystem::path GetExecutableDir() {
    return GetExecutablePath().parent_path();
}

std::filesystem::path GetResourcesPath() {
#if defined(_WIN32) && defined(DEBUG)
    return GetExecutableDir().parent_path() / "resources";
#else
    return GetExecutableDir() / "resources";
#endif
}
