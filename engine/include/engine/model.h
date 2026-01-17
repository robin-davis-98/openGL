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
    bool active;
};

struct ENGINE_API Model
{
    ModelPart parts[MAX_MODEL_PARTS];
    uint32_t partCount;
};

ENGINE_API Model model_Load(const std::string& filePath);
ENGINE_API Model model_Create(Vertex* vertices, uint32_t count, Material material);
void model_Draw(const Model& model);
void model_Destroy(Model& model);