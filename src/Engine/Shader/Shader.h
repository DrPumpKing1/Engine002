#pragma once

#include <glad/gl.h>
#include <glm/glm.hpp>

#include <string>

const unsigned int SHADER_INFO_LOG_SIZE = 1024;

enum class ShaderType {
    COMPUTE,
    VERTEX,
    TESS_CONTROL,
    TESS_EVAL,
    GEOMETRY,
    FRAGMENT
};

std::string ShaderTypeToString(const ShaderType &type);
GLenum ShaderTypeToGL(const ShaderType &type);

class Shader {
    private:
        GLuint ID;
        ShaderType type;
        std::string source; 

        mutable bool error;

    public:
        Shader(const std::string &path, const ShaderType &type);
        ~Shader();

        Shader(const Shader &other);
        Shader& operator=(const Shader &other);

        Shader(Shader &&other) noexcept;
        Shader& operator=(Shader &&other) noexcept;

        void AttachShader(GLuint program, bool &success) const;

        const ShaderType &GetType() const { return type; }

    private:
        void Compile();
        void CheckCompileErrors() const;

        bool IsValid() const;
};
