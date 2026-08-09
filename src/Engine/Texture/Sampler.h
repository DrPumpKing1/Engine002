#pragma once

#include <glad/gl.h>
#include <glm/glm.hpp>

class Sampler {
    private:
        GLuint ID;

        GLenum minFilter = GL_LINEAR_MIPMAP_LINEAR;
        GLenum magFilter = GL_LINEAR;
        GLenum wrapS = GL_REPEAT;
        GLenum wrapT = GL_REPEAT;
        float anisotropy = 1.0;
        glm::vec4 borderColor = glm::vec4(0.0);

        static float maxAnisotropy;

    public:
        Sampler();
        ~Sampler();
        
        Sampler(const Sampler&);
        Sampler& operator=(const Sampler&);

        Sampler(Sampler&& other) noexcept;
        Sampler& operator=(Sampler&& other) noexcept;

        void Bind(GLuint unit) const;

        // Config
        void SetMinFilter(GLenum filter);
        void SetMagFilter(GLenum filter);
        void SetWrapS(GLenum mode);
        void SetWrapT(GLenum mode);
        void SetAnisotropy(float level);
        void SetBorderColor(glm::vec4 color);

        GLenum GetMinFilter() const { return minFilter; }
        GLenum GetMagFilter() const { return magFilter; }
        GLenum GetWrapSMode() const { return wrapS; }
        GLenum GetWrapTMode() const { return wrapT; }
        float GetAnisotropyLevel() const { return anisotropy; }
        glm::vec4 GetBorderColor() const { return borderColor; }

    private:
        void Create();
        void Release();
        bool IsValid() const;
};
