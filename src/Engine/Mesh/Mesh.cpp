#include "Mesh.h"
#include "../../Utils/Logger/Logger.h"
#include "VertexAttribute.h"

Mesh::Mesh() : VAO(0), VBO(0), EBO(0), instances(0) {}

Mesh::Mesh(const VertexLayout &layout, const std::vector<float> &vertices, const std::vector<unsigned int> &indices, unsigned int instances) : layout(layout), vertices(vertices), indices(indices), instances(instances) {
    if(!CreateBuffers(nullptr)) {
        LOG("WARNING::MESH_CONSTRUCTOR, mesh could not be constructed");
    }
}

Mesh::~Mesh() {
    Release();
}

Mesh::Mesh(const Mesh &other) : layout(other.layout), vertices(other.vertices), indices(other.indices), instances(other.instances) {
    if(!CreateBuffers(&other)) {
        LOG("WARNING::MESH_COPY_CONSTRUCTOR, mesh could not be constructed from copy source");
    }
}

Mesh& Mesh::operator=(const Mesh &other) {
    if(this == &other) return *this;

    Release();
    layout = other.layout;
    vertices = other.vertices;
    indices = other.indices;
    instances = other.instances;

    if(!CreateBuffers(&other)) {
        LOG("WARNING::MESH_COPY_ASSIGNMENT, mesh could not be constructed from copy source");
    }

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

    Release();
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
    if(!IsValid())
        LOG("WARNING::MESH_BIND mesh is not valid to bind, vertex array buffer binding aborted");
    glBindVertexArray(VAO);
}

void Mesh::Unbind() {
    glBindVertexArray(0);
}

bool Mesh::CreateBuffers(const Mesh *source) {
    GLsizeiptr vertexSize = static_cast<GLsizeiptr>(vertices.size() * sizeof(float));
    GLsizeiptr elementsSize = static_cast<GLsizeiptr>(indices.size() * sizeof(unsigned int));

    if (vertexSize == 0 || elementsSize == 0) {
        VAO = VBO = EBO = 0;
        LOG("WARNING::MESH_ALLOCATION mesh has no vertices or indices, mesh voided");
        return false;
    }

    if (source && !source->IsValid()) {
        VAO = VBO = EBO = 0;
        LOG("WARNING::MESH_ALLOCATION source mesh is not valid, copy aborted, mesh voided");
        return false;
    }

    glCreateVertexArrays(1, &VAO);
    glCreateBuffers(1, &VBO);
    glCreateBuffers(1, &EBO);

    if (source) {
        glNamedBufferStorage(VBO, vertexSize, nullptr, 0);
        glNamedBufferStorage(EBO, elementsSize, nullptr, 0);
        glCopyNamedBufferSubData(source->VBO, VBO, 0, 0, vertexSize);
        glCopyNamedBufferSubData(source->EBO, EBO, 0, 0, elementsSize);
    } else {
        glNamedBufferStorage(VBO, vertexSize, vertices.data(), 0);
        glNamedBufferStorage(EBO, elementsSize, indices.data(), 0);
    }

    SetupVertexArrayLayout();
    draw = DrawInfo{static_cast<unsigned int>(indices.size()), instances};
    return true;
}

void Mesh::SetupVertexArrayLayout() {
    GLuint bindingPoint {0};
    GLsizei stride {0};
    for(const auto &attribute : layout) {
        stride += attribute.components * sizeof(float);
    }
    glVertexArrayVertexBuffer(VAO, bindingPoint, VBO, 0, stride);
    glVertexArrayElementBuffer(VAO, EBO);

    GLuint offset {0};
    for(size_t i = 0; i < layout.size(); i++) {
        const auto &attribute = layout[i];
        glEnableVertexArrayAttrib(VAO, i);
        glVertexArrayAttribFormat(VAO, i, attribute.components, GL_FLOAT, GL_FALSE, offset);
        glVertexArrayAttribBinding(VAO, i, bindingPoint);
        glVertexArrayBindingDivisor(VAO, i, attribute.divisor);
        offset += attribute.components * sizeof(float);
    }
}

void Mesh::Release() {
    if(VAO) glDeleteVertexArrays(1, &VAO);
    if(VBO) glDeleteBuffers(1, &VBO);
    if(EBO) glDeleteBuffers(1, &EBO);
    VAO = VBO = EBO = 0;
}

bool Mesh::IsValid() const {
    return VAO != 0 && VBO != 0 && EBO != 0;
}
