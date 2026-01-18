#include "engine/renderer.h"
#include <glad/glad.h>
#include <iostream>

RendererData rendererData = {};

void renderer_Initialize()
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);
    
    renderer_InitializeSSBO();
}

void renderer_Shutdown()
{

}

void renderer_RenderViewport(App& app)
{
    if (app.activeScene != nullptr)
    {
        scene_Render(*app.activeScene, app.viewportRenderTarget, app.window.clearColour);
    }
}

void renderer_InitializeSSBO()
{
    // Create Light Buffer
    glGenBuffers(1, &rendererData.lightSSBO);

    if (rendererData.lightSSBO == 0) {
        std::cout << "CRITICAL: glGenBuffers failed to create lightSSBO!" << std::endl;
        return;
    }

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, rendererData.lightSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(LightBuffer), nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, rendererData.lightSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    std::cout << "SUCCESS: lightSSBO created with ID: " << rendererData.lightSSBO << std::endl;
}

void renderer_UpdateLights(Scene& scene)
{
    rendererData.lightData.count = 0;

    // Loop through scene nodes to find lights (assuming you have a way to identify them)
    // For now, let's just use your hardcoded values but ensure they are visible
    rendererData.lightData.lights[0].position = glm::vec4(2.0f, 5.0f, 0.0f, 15.0f); // 15 unit radius
    rendererData.lightData.lights[0].colour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);    // White, 1.0 intensity
    rendererData.lightData.count = 1;

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, rendererData.lightSSBO);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(LightBuffer), &rendererData.lightData);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, rendererData.lightSSBO);
}