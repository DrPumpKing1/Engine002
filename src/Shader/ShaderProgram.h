#pragma once

#include "Shader.h"
#include "ShaderUniforms.h"

#include <unordered_map>

class ShaderProgram {
    private:
        GLuint ID;
        std::unordered_map<ShaderType, Shader> attachments;
        ShaderUniforms uniforms;

        bool compiled;
        mutable bool error;

    public:
        ShaderProgram();
        ~ShaderProgram();

        void AttachShader(Shader &&shader);
        void Compile();

        ShaderProgram(const ShaderProgram &other);
        ShaderProgram& operator=(const ShaderProgram &other);
        
        ShaderProgram(ShaderProgram &&other) noexcept;
        ShaderProgram& operator=(ShaderProgram &&other) noexcept;

        void Bind() const;
        static void Unbind();

        void SetBool(const std::string &name, bool value) const;
        void SetInt(const std::string &name, int value) const;
        void SetFloat(const std::string &name, float value) const;
        void SetVec2(const std::string &name, const glm::vec2 &value) const;
        void SetVec2(const std::string &name, float x, float y) const;
        void SetVec3(const std::string &name, const glm::vec3 &value) const;
        void SetVec3(const std::string &name, float x, float y, float z) const;
        void SetVec4(const std::string &name, const glm::vec4 &value) const;
        void SetVec4(const std::string &name, float x, float y, float z, float w) const;
        void SetMat2(const std::string &name, const glm::mat2 &mat) const;
        void SetMat3(const std::string &name, const glm::mat3 &mat) const;
        void SetMat4(const std::string &name, const glm::mat4 &mat) const;

    private:
        void CheckLinkErrors() const;

        bool HasBeenCompiled() const;
        bool IsValid() const;
};
