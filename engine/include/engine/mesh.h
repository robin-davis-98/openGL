#pragma once
#include <cstdint>
#include <glm/glm.hpp>
#include "engine/engine.h"

struct ENGINE_API Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 textureCoords;
};

struct Mesh
{
    uint32_t vertexArray;
    uint32_t vertexBuffer;
    uint32_t elementBuffer;
    uint32_t indexCount;
};

Mesh mesh_Create(Vertex* vertices, uint32_t vertexCount, uint32_t* indices, uint32_t indexCount);
void mesh_Destroy(Mesh& mesh);