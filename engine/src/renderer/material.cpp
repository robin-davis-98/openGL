#include "engine/material.h"

Material material_Create(Shader* shader)
{
    Material mat;
    mat.shader = shader;
    return mat;
}

void material_Destroy(Material& material)
{
    material.shader = nullptr;
    material = {};
}