#pragma once

#include <string>

class FileReader {
public:
    static std::string ReadFile(const std::string &filePath);
};
