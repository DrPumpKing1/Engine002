#include "Shape.h"

void Shape::AddVertex(std::vector<float> &vertices, const VertexAttributeTypeLayout &attributes, glm::vec3 position, glm::vec2 texCoords, glm::vec3 normal, glm::vec3 tangent, glm::vec3 bitangent) const {
    for (auto attribType : attributes)
    {
        switch (attribType)
        {
            case VertexAttributeType::POSITION2D:
                vertices.push_back(position[0]);
                vertices.push_back(position[1]);
                break;
            case VertexAttributeType::POSITION3D:
                vertices.push_back(position[0]);
                vertices.push_back(position[1]);
                vertices.push_back(position[2]);
                break;
            case VertexAttributeType::TEXCOORD:
                vertices.push_back(texCoords[0]);
                vertices.push_back(texCoords[1]);
                break;
            case VertexAttributeType::NORMAL:
                vertices.push_back(normal[0]);
                vertices.push_back(normal[1]);
                vertices.push_back(normal[2]);
                break;
            case VertexAttributeType::TANGENT:
                vertices.push_back(tangent[0]);
                vertices.push_back(tangent[1]);
                vertices.push_back(tangent[2]);
                break;
            case VertexAttributeType::BITANGENT:
                vertices.push_back(bitangent[0]);
                vertices.push_back(bitangent[1]);
                vertices.push_back(bitangent[2]);
                break;
        }
    }
}
