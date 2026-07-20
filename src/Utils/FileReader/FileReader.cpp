#include "FileReader.h"

const std::string &FileReader::ReadFile(const std::string &filePath) {
    static const std::string empty = "";
    std::ifstream file(filePath);
    if(!file) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return empty;
    }
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        std::stringstream buffer;
        buffer << file.rdbuf();
        file.close();
        static const std::string content = buffer.str();
        return content;
    } catch(std::ifstream::failure &exception) {
        std::cerr << "Error reading from file: " << exception.what() << std::endl;
        return empty;
    }
}