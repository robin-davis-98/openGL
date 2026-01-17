#pragma once
#include <cstdint>
#include "engine/shader.h"
#include "engine/engine.h"
#include "engine/model.h"
#include "engine/renderTarget.h"

#define MAX_SCENE_MODELS 32

struct ENGINE_API Scene
{
    Model models[MAX_SCENE_MODELS];
    uint32_t modelCount = 0;
};

ENGINE_API Scene scene_Create();

ENGINE_API void scene_AddModel(Scene& scene, const Model& model);
ENGINE_API void scene_Render(Scene& scene, RenderTarget& target);
ENGINE_API void scene_Destroy(Scene& scene);