#pragma once 

#include <glad/gl.h>
#include <glm/glm.hpp>

#include <string>

class UniformBlock {
    private:
        GLuint ID;
        std::string name;

        GLuint bindingPoint;
        size_t size;

        bool lazy;
    public:
        UniformBlock(const std::string &name, GLuint bindingPoint = 0, size_t size = 0, const void *data = nullptr);
        ~UniformBlock();

        UniformBlock(const UniformBlock &other);
        UniformBlock& operator=(const UniformBlock &other);

        UniformBlock(UniformBlock &&other) noexcept;
        UniformBlock& operator=(UniformBlock &&other) noexcept;

        void Bind() const;
        void Unbind() const;

        void UpdateData(const void *data, size_t size = 0, size_t offset = 0);

        const std::string &GetName() const { return name; }
        GLuint GetBindingPoint() const { return bindingPoint; }

    private:
        bool IsValid() const;
        void Initialize(const void *data);
};

struct ShaderUniformBlockBinding {
    GLuint bindingPoint;
    GLuint blockIndex;
    std::string uniformBlockName;
};
