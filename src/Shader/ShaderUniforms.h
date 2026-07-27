#pragma once

#include <glad/gl.h>
#include <glm/glm.hpp>

#include <string>
#include <unordered_map>

const int UNIFORM_CACHE_INITIAL_SIZE = 4;

class ShaderUniforms {
    private:
        GLuint program;

        mutable std::unordered_map<std::string, GLint> locations;

    public:
        ShaderUniforms();

        void SetShaderProgram(GLuint program);

        GLint GetUniformLocation(const std::string &name) const;

    private:
        bool IsProgramSet() const;
};
