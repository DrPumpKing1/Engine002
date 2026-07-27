#include "ShaderProgram.h"
#include <iostream>

ShaderProgram::ShaderProgram() : compiled(false), error(false) {
    ID = glCreateProgram();
    uniforms.SetShaderProgram(ID);
}

ShaderProgram::~ShaderProgram() {
    attachments.clear();
    glDeleteProgram(ID);
}

ShaderProgram::ShaderProgram(const ShaderProgram &other) {
    if(!other.IsValid()) {
        ID = 0;
        compiled = false;
        error = false;
        uniforms.SetShaderProgram(0);
    } else {
        ID = glCreateProgram();
        uniforms.SetShaderProgram(ID);
        for(const auto &[shaderType, shader] : other.attachments) {
            Shader copyShader = shader;
            AttachShader(std::move(copyShader));
        }
        if(other.HasBeenCompiled())
            Compile();
    }
}

ShaderProgram& ShaderProgram::operator=(const ShaderProgram &other) {
    if(this == &other) return *this;

    attachments.clear();
    glDeleteProgram(ID);
    compiled = false;
    error = false;

    if(!other.IsValid()) {
        ID = 0;
        compiled = false;
        error = false;
        uniforms.SetShaderProgram(0);
    } else {
        ID = glCreateProgram();
        for(const auto &[shaderType, shader] : other.attachments) {
            Shader copyShader = shader;
            AttachShader(std::move(copyShader));
        }
        if(other.HasBeenCompiled())
            Compile();
        uniforms.SetShaderProgram(ID);
    }

    return *this;
}

ShaderProgram::ShaderProgram(ShaderProgram &&other) noexcept {
    if(!other.IsValid()) {
        ID = 0;
        compiled = false;
        error = false;
        uniforms.SetShaderProgram(0);
    } else {
        ID = other.ID;
        attachments = std::move(other.attachments);
        compiled = other.compiled;
        error = other.error;
        uniforms.SetShaderProgram(ID);

        other.ID = 0;
        other.compiled = false;
        other.error = false;
        other.uniforms.SetShaderProgram(0);
    }
}

ShaderProgram& ShaderProgram::operator=(ShaderProgram &&other) noexcept {
    if(this == &other) return *this;

    attachments.clear();
    glDeleteProgram(ID);
    compiled = false;
    error = false;

    if(!other.IsValid()) {
        ID = 0;
        compiled = false;
        error = false;
        uniforms.SetShaderProgram(0);
    } else {
        ID = other.ID;
        attachments = std::move(other.attachments);
        compiled = other.compiled;
        error = other.error;
        uniforms.SetShaderProgram(ID);

        other.ID = 0;
        other.compiled = false;
        other.error = false;
        other.uniforms.SetShaderProgram(0);
    }

    return *this;
}

void ShaderProgram::AttachShader(Shader &&shader) {
    const ShaderType& shaderType = shader.GetType();
    auto it = attachments.find(shaderType);
    if(it == attachments.end()) {
        bool success;
        shader.AttachShader(ID, success);
        if(success) {
            attachments.emplace(shaderType, std::move(shader));
        } else {
            std::cerr << "ERROR::SHADER_PROGRAM_ATTACHMENT failed of type: " << ShaderTypeToString(shaderType) << "\n Shader attachment failed, shader was not valid or had erros" << std::endl;
        }
    } else {
        std::cerr << "ERROR::SHADER_PROGRAM_ATTACHMENT failed of type: " << ShaderTypeToString(shaderType) << "\n Another shader of the same type has been already attached" << std::endl;
    }
}

void ShaderProgram::Compile() {
    compiled = true;
    glLinkProgram(ID);
    CheckLinkErrors();
}

void ShaderProgram::Bind() const {
    glUseProgram(ID);
}

void ShaderProgram::Unbind() {
    glUseProgram(0);
}

void ShaderProgram::CheckLinkErrors() const {
    int success;
    char infoLog[SHADER_INFO_LOG_SIZE];
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success) {
        error = true;
        glGetProgramInfoLog(ID, SHADER_INFO_LOG_SIZE, nullptr, infoLog);
        std::cerr << "ERROR::SHADER_PROGRAM_LINKING failed:\n" << infoLog << std::endl;
    }
}

bool ShaderProgram::HasBeenCompiled() const {
    return compiled;
}

bool ShaderProgram::IsValid() const {
    return ID != 0;
}
