#pragma once
#include <cstdint>
#include "engine/shader.h"
#include "engine/engine.h"

struct ENGINE_API Material
{
    Shader* shader;
};

ENGINE_API Material material_Create(Shader* shader);
ENGINE_API void material_Destroy(Material& material);