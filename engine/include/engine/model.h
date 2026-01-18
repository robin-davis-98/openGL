#pragma once

#include <cstdint>
#include "engine/engine.h"
#include "engine/mesh.h"
#include "engine/material.h"

#define MAX_MODEL_PARTS 8

struct ModelPart
{
    Mesh mesh;
    Material material;
    glm::vec3 position;
    bool active;
};

struct ENGINE_API Model
{
    ModelPart parts[MAX_MODEL_PARTS];
    uint32_t partCount;
};

ENGINE_API Model model_Load(const std::string& filePath);
ENGINE_API Model model_Create(Vertex* vertices, uint32_t vertexCount, uint32_t* indices, uint32_t indexCount, Material material);
void model_Draw(const Model* model, const glm::mat4& transform);
void model_DrawWithShader(const Model* model, const glm::mat4& transform, Shader overrideShader);
void model_Destroy(Model* model);