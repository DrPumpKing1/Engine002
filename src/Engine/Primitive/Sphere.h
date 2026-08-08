#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

#include "Shape.h"

class Sphere : public Shape {
    private:
        float radius;
        unsigned int segments;
        unsigned int rings;
    public:
        Sphere(const VertexAttributeTypeLayout &attributes, float radius, unsigned int segments, unsigned int rings);
    private:
        Mesh ConstructMesh(const VertexAttributeTypeLayout &attributes) const override;
};
