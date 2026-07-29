#include "Mesh.h"

Mesh::Mesh(const VertexLayout &layout, const std::vector<float> &vertices, const std::vector<unsigned int> &indices, unsigned int instances) : layout(layout), vertices(vertices), indices(indices), instances(instances) {
    Initialize();
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

Mesh::Mesh(const Mesh &other) : layout(other.layout), vertices(other.vertices), indices(other.indices), instances(other.instances) {
    Initialize();
}

Mesh& Mesh::operator=(const Mesh &other) {
    if(this == &other) return *this;

    layout = other.layout;
    vertices = other.vertices;
    indices = other.indices;
    instances = other.instances;
    Initialize();

    return *this;
}

Mesh::Mesh(Mesh &&other) noexcept : layout(std::move(other.layout)), vertices(std::move(other.vertices)), indices(std::move(other.indices)), instances(other.instances), VAO(other.VAO), VBO(other.VBO), EBO(other.EBO), draw(std::move(other.draw)) {
    other.instances = 0;
    other.VAO = 0;
    other.VBO = 0;
    other.EBO = 0;
}

Mesh& Mesh::operator=(Mesh &&other) noexcept {
    if(this == &other) return *this;

    layout = std::move(other.layout);
    vertices = std::move(other.vertices);
    indices = std::move(other.indices);
    instances = other.instances;

    VAO = other.VAO;
    VBO = other.VBO;
    EBO = other.EBO;

    draw = std::move(other.draw);

    other.instances = 0;
    other.VAO = 0;
    other.VBO = 0;
    other.EBO = 0;

    return *this;
}

void Mesh::Bind() const {
    glBindVertexArray(VAO);
}

void Mesh::Unbind() {
    glBindVertexArray(0);
}

void Mesh::Initialize() {
    glCreateVertexArrays(1, &VAO);
    glCreateBuffers(1, &VBO);
    glCreateBuffers(1, &EBO);

    glNamedBufferStorage(VBO, vertices.size() * sizeof(float), vertices.data(), 0);
    glNamedBufferStorage(EBO, indices.size() * sizeof(unsigned int), indices.data(), 0);

    GLuint bindingPoint {0};
    size_t stride {0};
    for(const auto &attribute : layout) {
        stride += attribute.components * sizeof(float);
    }
    glVertexArrayVertexBuffer(VAO, bindingPoint, VBO, 0, stride);
    glVertexArrayElementBuffer(VAO, EBO);

    size_t offset {0};
    for(int i = 0; i < layout.size(); i++) {
        const auto &attribute = layout[i];
        glEnableVertexArrayAttrib(VAO, i);
        glVertexArrayAttribFormat(VAO, i, attribute.components, GL_FLOAT, GL_FALSE, offset);
        glVertexArrayAttribBinding(VAO, i, bindingPoint);
        glVertexArrayBindingDivisor(VAO, i, attribute.divisor);
        offset += attribute.components * sizeof(float);
    }

    unsigned int indicesCount = indices.size();
    draw = DrawInfo{indicesCount, instances};
}
