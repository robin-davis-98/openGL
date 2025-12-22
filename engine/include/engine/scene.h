#pragma once
#include <cstdint>
#include "engine/shader.h"
#include "engine/renderTarget.h"

struct Scene
{
    Shader shader;
    uint32_t vertexArray = 0;
    uint32_t vertexBuffer = 0;
};

void scene_Initialize(Scene& scene);
void scene_Render(Scene& scene, RenderTarget& target);
void scene_Destroy(Scene& scene);