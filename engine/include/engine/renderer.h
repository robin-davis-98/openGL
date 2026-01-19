#pragma once

#include "engine/app.h"
#include "engine/light.h"
#include "engine/shader.h"

struct UIVertex
{
    glm::vec2 position;
    glm::vec2 uv;
};

struct UIButton
{
    glm::vec2 pos;
    glm::vec2 size;
    glm::vec4 colour;
    bool isHovered;
};

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

    glm::vec2 currentResolution;

    glm::uvec3 gridSize;
    uint32_t clusterCount;

    uint32_t clusterSSBO;
    Cluster clusterData[3456];

    Shader clusterBuildShader;
    Shader cullShader;

    struct {
        Shader shader;
        uint32_t vao, vbo;
        glm::mat4 projection;
    } uiData;
};

extern RendererData rendererData;

void renderer_Initialize();
void renderer_Shutdown();
void renderer_RenderViewport(App& app);
void renderer_UpdateLights(Scene& scene);
void renderer_BuildClusters(float screenWidth, float screenHeight, uint32_t& cameraUBO);
void renderer_InitializeSSBO();
void renderer_checkBufferData();

void renderer_InitUI();
void ui_DrawRect(glm::vec2 pos, glm::vec2 size, glm::vec4 colour, uint32_t textureID = 0);
void ui_DrawHealthBar(glm::vec2 pos, glm::vec2 size, float healthPercent);
bool ui_IsPointInRect(glm::vec2 point, glm::vec2 pos, glm::vec2 size);
void renderer_BeginUI(int width, int height);
void renderer_EndUI();