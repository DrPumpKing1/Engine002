#pragma once

#include <glad/gl.h>

#include "VertexAttribute.h"

struct DrawInfo {
    unsigned int indices;
    unsigned int instances;
};

class Mesh {
    private:
        VertexLayout layout;
        std::vector<float> vertices;
        std::vector<unsigned int> indices;

        GLuint VAO, VBO, EBO;
        unsigned int instances;

        DrawInfo draw;

    public:
        Mesh(const VertexLayout &layout, const std::vector<float> &vertices, const std::vector<unsigned int> &indices, unsigned int instances = 0);
        ~Mesh();

        Mesh(const Mesh& other);
        Mesh& operator=(const Mesh& other);

        Mesh(Mesh &&other) noexcept;
        Mesh& operator=(Mesh &&ohter) noexcept;

        void Bind() const;
        static void Unbind();

        const DrawInfo &GetDrawInfo() const { return draw; }

    private:
        void Initialize();
};
