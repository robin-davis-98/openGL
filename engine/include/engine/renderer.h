#pragma once

#include "engine/app.h"
#include "engine/light.h"

struct Cluster
{
    glm::vec4 minPoint;
    glm::vec4 maxPoint;
};

struct RendererData
{
    uint32_t lightSSBO;
    LightBuffer lightData;
};

extern RendererData rendererData;

void renderer_Initialize();
void renderer_Shutdown();
void renderer_RenderViewport(App& app);
void renderer_UpdateLights(Scene& scene);
void renderer_InitializeSSBO();