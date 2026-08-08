#include "Sphere.h"

Sphere::Sphere(const VertexAttributeTypeLayout &attributes, float radius, unsigned int segments, unsigned int rings) : Shape(attributes), radius(radius), segments(segments), rings(rings) {
    Build();
}

Mesh Sphere::ConstructMesh(const VertexAttributeTypeLayout &attributes) const {
    VertexLayout layout = VertexAttribute::GetVertexLayout(attributes);

    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    for (int ring = 0; ring <= rings; ring++)
    {
        float v = static_cast<float>(ring) / static_cast<float>(rings);
        float theta = v * M_PI;

        float sinTheta = std::sin(theta);
        float cosTheta = std::cos(theta);

        for (int seg = 0; seg <= segments; seg++)
        {
            float u = static_cast<float>(seg) / static_cast<float>(segments);
            float phi = u * 2.0f * M_PI;

            float sinPhi = std::sin(phi);
            float cosPhi = std::cos(phi);

            glm::vec3 position(radius * sinTheta * cosPhi,
                               radius * cosTheta,
                               radius * sinTheta * sinPhi);

            glm::vec2 texCoords(u, v);

            glm::vec3 normal = glm::normalize(position);

            glm::vec3 tangent(
                -radius * sinTheta * sinPhi,
                0.0f,
                radius * sinTheta * cosPhi);

            if (glm::length(tangent) < 1e-6f)
            {
                glm::vec3 fallback = glm::abs(normal.y) < 0.999f ? glm::vec3(0.0, 1.0, 0.0) : glm::vec3(1.0, 0.0, 0.0);
                tangent = glm::cross(fallback, normal);
            }
            tangent = glm::normalize(tangent);

            glm::vec3 bitangent(
                radius * cosTheta * cosPhi,
                -radius * sinTheta,
                radius * cosTheta * sinPhi);
            bitangent = glm::normalize(bitangent);

            tangent = glm::normalize(tangent - normal * glm::dot(normal, tangent));
            bitangent = glm::cross(normal, tangent);

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
    }

    int rows = segments + 1;
    for (int ring = 0; ring < rings; ring++)
    {
        for (int seg = 0; seg < segments; seg++)
        {
            int current = ring * rows + seg;
            int next = current + rows;

            indices.push_back(current);
            indices.push_back(current + 1);
            indices.push_back(next);

            indices.push_back(current + 1);
            indices.push_back(next + 1);
            indices.push_back(next);
        }
    }

    return Mesh(layout, vertices, indices);
}
