#pragma once

#include "engine/app.h"
#include "engine/light.h"
#include "engine/shader.h"

struct Cluster
{
    glm::vec4 minPoint;
    glm::vec4 maxPoint;
    
    uint32_t count;
    uint32_t lightIndices[100];
    uint32_t _padding[3];
};

struct RendererData
{
    uint32_t lightSSBO;
    LightBuffer lightData;

    uint32_t clusterSSBO;
    Cluster clusterData[3456];

    Shader clusterBuildShader;
    Shader cullShader;
};

extern RendererData rendererData;

void renderer_Initialize();
void renderer_Shutdown();
void renderer_RenderViewport(App& app);
void renderer_UpdateLights(Scene& scene);
void renderer_InitializeSSBO();