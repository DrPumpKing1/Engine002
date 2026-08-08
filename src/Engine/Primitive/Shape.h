#pragma once

#include <glm/glm.hpp>

#include "../Mesh/Mesh.h"

class Shape {
    private:
        Mesh mesh;
        const VertexAttributeTypeLayout &attributes;
    public:
        explicit Shape(const VertexAttributeTypeLayout &attributes) : attributes(attributes) {
        }

        virtual ~Shape() = default;

        const Mesh &GetMesh() const { return mesh; }
    protected:
        void Build() { mesh = ConstructMesh(attributes); }
        void AddVertex(std::vector<float> &vertices, const VertexAttributeTypeLayout &attributes, glm::vec3 position, glm::vec2 texCoords, glm::vec3 normal, glm::vec3 tangent, glm::vec3 bitangent) const;
    private:
        virtual Mesh ConstructMesh(const VertexAttributeTypeLayout &attributes) const = 0;
};
