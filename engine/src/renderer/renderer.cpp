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

void collect_Lights_Recursive(Node* node, LightBuffer& buffer)
{
    if (node->type == NodeType::Light && buffer.count < 1024)
    {
        PointLight& pl = buffer.lights[buffer.count];
        
        // Use the world matrix to get the actual position in the scene
        pl.position = glm::vec4(glm::vec3(node->worldMatrix[3]), node->light.radius);
        pl.colour = glm::vec4(node->light.colour, node->light.intensity);
        
        buffer.count++;
    }

    for (Node* child : node->children)
    {
        collect_Lights_Recursive(child, buffer);
    }
}

void renderer_UpdateLights(Scene& scene)
{

    rendererData.lightData.count = 0;

    if (scene.root)
    {
        collect_Lights_Recursive(scene.root, rendererData.lightData);
    }

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, rendererData.lightSSBO);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(LightBuffer), &rendererData.lightData);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, rendererData.lightSSBO);
}