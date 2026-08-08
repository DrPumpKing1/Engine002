#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

#include "Shape.h"

class Cube : public Shape {
    private:
        float length;
        unsigned int xSubdivisions;
        unsigned int ySubdivisions;
        unsigned int zSubdivisions;
    public:
        Cube(const VertexAttributeTypeLayout &attributes, float length, unsigned int xSubdivisions, unsigned int ySubdivisions, unsigned int zSubdivisions);
    private:
        Mesh ConstructMesh(const VertexAttributeTypeLayout &attributes) const override;
};
