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
    private:
        virtual Mesh ConstructMesh(const VertexAttributeTypeLayout &attributes) const = 0;
};
