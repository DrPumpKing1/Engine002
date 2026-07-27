#include "Shader.h"
#include "../Utils/FileReader/FileReader.h"

std::string ShaderTypeToString(const ShaderType &type) {
    switch (type) {
        case ShaderType::COMPUTE:
            return "COMPUTE";
        case ShaderType::VERTEX:
            return "VERTEX";
        case ShaderType::TESS_CONTROL:
            return "TESS_CONTROL";
        case ShaderType::TESS_EVAL:
            return "TESS_EVALUATION";
        case ShaderType::GEOMETRY:
            return "GEOMETRY";
        case ShaderType::FRAGMENT:
            return "FRAGMENT";
        default:
            return "UNKNOWN";
    }
}

GLenum ShaderTypeToGL(const ShaderType &type) {
    switch (type) {
        case ShaderType::COMPUTE:
            return GL_COMPUTE_SHADER;
        case ShaderType::VERTEX:
            return GL_VERTEX_SHADER;
        case ShaderType::TESS_CONTROL:
            return GL_TESS_CONTROL_SHADER;
        case ShaderType::TESS_EVAL:
            return GL_TESS_EVALUATION_SHADER;
        case ShaderType::GEOMETRY:
            return GL_GEOMETRY_SHADER;
        case ShaderType::FRAGMENT:
            return GL_FRAGMENT_SHADER;
        default:
            throw std::invalid_argument("Unknown shader type");
    }
}

Shader::Shader(const std::string &path, const ShaderType &type) : type(type), error(false){
    source = FileReader::ReadFile(path);
    Compile();
}

Shader::~Shader() {
    if(IsValid())
        glDeleteShader(ID);
}

Shader::Shader(const Shader &other) : type(other.type), source(other.source), error(false) {
    Compile();
}

Shader& Shader::operator=(const Shader &other) {
    if(this == &other) return *this;

    if(IsValid())
        glDeleteShader(ID);

    source = other.source;
    type = other.type;
    Compile();

    return *this;
}

Shader::Shader(Shader &&other) noexcept : ID(other.ID), type(other.type), source(std::move(other.source)), error(other.error) {
    other.ID = 0;
}

Shader& Shader::operator=(Shader &&other) noexcept {
    if(this == &other) return *this;

    if(IsValid())
        glDeleteShader(ID);

    ID = other.ID;
    type = other.type;
    source = std::move(other.source);
    other.ID = 0;

    return *this;
}

void Shader::AttachShader(GLuint program, bool &success) const {
    success = IsValid() && !error;

    if(success)
        glAttachShader(program, ID);
}

void Shader::Compile() {
    const char* sourceStr = source.c_str();
    ID = glCreateShader(ShaderTypeToGL(type));
    glShaderSource(ID, 1, &sourceStr, nullptr);
    glCompileShader(ID);
    CheckCompileErrors();
}

void Shader::CheckCompileErrors() const {
    int success;
    char infoLog[SHADER_INFO_LOG_SIZE];
    glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
    if(!success) {
        error = true;
        glGetShaderInfoLog(ID, SHADER_INFO_LOG_SIZE, nullptr, infoLog);
        std::cerr <<  "ERROR::SHADER_COMPILATION failed of type: " << ShaderTypeToString(type) << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl; 
    }
}

bool Shader::IsValid() const {
    return ID != 0;
}
