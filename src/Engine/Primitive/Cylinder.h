#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

#include "Shape.h"

class Cylinder : public Shape {
    private:
        float radius;
        float height;
        unsigned int segments;
        unsigned int rings;
    public:
        Cylinder(const VertexAttributeTypeLayout &attributes, float radius, float height, unsigned int segments, unsigned int rings);
    private:
        Mesh ConstructMesh(const VertexAttributeTypeLayout &attributes) const override;
};
