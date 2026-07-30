#include "UniformBlock.h"
#include <iostream>

UniformBlock::UniformBlock(const std::string &name, GLuint bindingPoint, size_t size, const void *data) : name(name), bindingPoint(bindingPoint), size(size), lazy(false) {
    if(size == 0) {
        std::cout << "WARNING::UNIFORM_BLOCK of name: " << name << " have zero size, defaulted to lazy initialization" << std::endl; 
        ID = 0;
        lazy = true;
        return;
    }
    SetupStorage(data);
}

UniformBlock::~UniformBlock() {
    if(!lazy && IsValid())
        glDeleteBuffers(1, &ID);
}

UniformBlock::UniformBlock(const UniformBlock &other) : ID(0), name(other.name), bindingPoint(other.bindingPoint), size(other.size), lazy(other.lazy) {
    if(!lazy) {
        glCreateBuffers(1, &ID);
        glCopyNamedBufferSubData(other.ID, ID, 0, 0, size);
    }
}

UniformBlock& UniformBlock::operator=(const UniformBlock &other) {
    if(this == &other) return *this;

    if(!lazy && IsValid())
        glDeleteBuffers(1, &ID);

    ID = 0;
    name = other.name;
    bindingPoint = other.bindingPoint;
    size = other.size;
    lazy = other.lazy;

    if(!lazy) {
        glCreateBuffers(1, &ID);
        glCopyNamedBufferSubData(other.ID, ID, 0, 0, size);
    }

    return *this;
}

UniformBlock::UniformBlock(UniformBlock &&other) noexcept : ID(other.ID), name(std::move(other.name)), bindingPoint(other.bindingPoint), size(other.size), lazy(other.lazy) {
    other.ID = 0;
    other.size = 0;
    other.lazy = false;
}

UniformBlock& UniformBlock::operator=(UniformBlock &&other) noexcept {
    if(this == &other) return *this;

    if(!lazy && IsValid())
        glDeleteBuffers(1, &ID);

    ID = other.ID;
    name = std::move(other.name);
    bindingPoint = other.bindingPoint;
    size = other.size;
    lazy = other.lazy;

    other.ID = 0;
    other.size = 0;
    other.lazy = false;

    return *this;
}

void UniformBlock::Bind() const {
    if(lazy) {
        std::cout << "WARNING::UNIFORM_BLOCK_BIND it has not been initialized yet";
        return;
    }
    glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, ID);
}

void UniformBlock::Unbind() const {
    glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, 0);
}

void UniformBlock::UpdateData(const void *data, size_t dataSize, size_t offset) {
    if(dataSize == 0) {
        std::cout << "WARNING::UNIFORM_BLOCK_UPDATE_DATA of name: " << name << " is trying to be updated with size zero data, aborting update." << std::endl; 
        return;
    }
    if(lazy) {
        size = dataSize + offset;
        SetupStorage(nullptr);
    } else if(!IsValid()) {
        std::cout << "WARNING::UNIFORM_BLOCK_UPDATE_DATA of name: " << name << " is not valid to update, aborting update." << std::endl; 
        return;
    }
    if(dataSize + offset > size) {
        std::cout << "WARNING::UNIFORM_BLOCK_UPDATE_DATA of name: " << name << " size and offset to update are out of bounds of buffer, aborting update." << std::endl; 
        return;
    }
    glNamedBufferSubData(ID, offset, dataSize, data);
}

bool UniformBlock::IsValid() const {
    return ID != 0;
}

void UniformBlock::SetupStorage(const void *data) {
    lazy = false;
    glCreateBuffers(1, &ID);
    glNamedBufferStorage(ID, size, data, GL_DYNAMIC_STORAGE_BIT);
}
