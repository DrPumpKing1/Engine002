#include "Sampler.h"

float Sampler::maxAnisotropy = 0.0f;

Sampler::Sampler() {
    Create();
}

Sampler::~Sampler() {
    Release();
}

Sampler::Sampler(const Sampler &other) : minFilter(other.minFilter), magFilter(other.magFilter), wrapS(other.wrapS), wrapT(other.wrapT), anisotropy(other.anisotropy), borderColor(other.borderColor) {
    Create();
}

Sampler& Sampler::operator=(const Sampler &other) {
    if(this == &other) return *this;

    if(IsValid())
        Release();

    minFilter = other.minFilter;
    magFilter = other.magFilter;
    wrapS = other.wrapS;
    wrapT = other.wrapT;
    anisotropy = other.anisotropy;
    borderColor = other.borderColor;
    Create();

    return *this;
}

Sampler::Sampler(Sampler &&other) noexcept : ID(other.ID), minFilter(other.minFilter), magFilter(other.magFilter), wrapS(other.wrapS), wrapT(other.wrapT), anisotropy(other.anisotropy), borderColor(other.borderColor) {
    other.ID = 0;
}

Sampler& Sampler::operator=(Sampler &&other) noexcept {
    if(this == &other) return *this;

    if(IsValid())
        Release();

    ID = other.ID;
    minFilter = other.minFilter;
    magFilter = other.magFilter;
    wrapS = other.wrapS;
    wrapT = other.wrapT;
    anisotropy = other.anisotropy;
    borderColor = other.borderColor;

    other.ID = 0;

    return *this;
}

void Sampler::Bind(GLuint unit) const {
    glBindSampler(unit, ID);
}

void Sampler::SetMinFilter(GLenum filter) {
    minFilter = filter;
    glSamplerParameteri(ID, GL_TEXTURE_MIN_FILTER, minFilter);
}

void Sampler::SetMagFilter(GLenum filter) {
    magFilter = filter;
    glSamplerParameteri(ID, GL_TEXTURE_MAG_FILTER, magFilter);
}

void Sampler::SetWrapS(GLenum mode) {
    wrapS = mode;
    glSamplerParameteri(ID, GL_TEXTURE_WRAP_S, wrapS);
}

void Sampler::SetWrapT(GLenum mode) {
    wrapT = mode;
    glSamplerParameteri(ID, GL_TEXTURE_WRAP_T, wrapT);
}

void Sampler::SetAnisotropy(float level) {
    anisotropy = level;
    if(!maxAnisotropy)
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &maxAnisotropy);
    glSamplerParameterf(ID, GL_TEXTURE_MAX_ANISOTROPY, std::min(anisotropy, maxAnisotropy));
}

void Sampler::SetBorderColor(glm::vec4 color) {
    borderColor = color;
    glSamplerParameterfv(ID, GL_TEXTURE_BORDER_COLOR, &borderColor[0]);
}

void Sampler::Create() {
    glCreateSamplers(1, &ID);
    glSamplerParameteri(ID, GL_TEXTURE_MIN_FILTER, minFilter);
    glSamplerParameteri(ID, GL_TEXTURE_MAG_FILTER, magFilter);
    glSamplerParameteri(ID, GL_TEXTURE_WRAP_S, wrapS);
    glSamplerParameteri(ID, GL_TEXTURE_WRAP_T, wrapT);
    if(!maxAnisotropy)
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &maxAnisotropy);
    glSamplerParameterf(ID, GL_TEXTURE_MAX_ANISOTROPY, std::min(anisotropy, maxAnisotropy));
    glSamplerParameterfv(ID, GL_TEXTURE_BORDER_COLOR, &borderColor[0]);
}

void Sampler::Release() {
    if(ID) glDeleteSamplers(1, &ID);
    ID = 0;
}

bool Sampler::IsValid() const {
    return ID != 0;
}
