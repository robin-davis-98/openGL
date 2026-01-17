#pragma once
#include <cstdint>
#include "engine/engine.h"

struct ENGINE_API Vertex
{
    float position[3];
};

struct Mesh
{
    uint32_t vertexArray;
    uint32_t vertexBuffer;
    uint32_t vertexCount;
};

Mesh mesh_Create(Vertex* vertices, uint32_t vertexCount);
void mesh_Destroy(Mesh& mesh);