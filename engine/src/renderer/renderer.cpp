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

    rendererData.clusterBuildShader = shader_CreateCompute("assets/shaders/default/cluster_build.comp");
    rendererData.cullShader = shader_CreateCompute("assets/shaders/default/cluster_cull.comp");

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

    glGenBuffers(1, &rendererData.clusterSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, rendererData.clusterSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Cluster) * 3456, nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, rendererData.clusterSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
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

void renderer_BuildClusters(float screenWidth, float screenHeight)
{

    shader_SetUI3(rendererData.clusterBuildShader, "uGridSize", 16, 9, 24);
    shader_SetVec2(rendererData.clusterBuildShader, "uScreenSize", glm::vec2(screenWidth, screenHeight));

    shader_Dispatch(16, 9, 24);
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

    shader_Use(rendererData.cullShader);

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, rendererData.lightSSBO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, rendererData.clusterSSBO);

    glDispatchCompute(1, 1, 24);

    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

void renderer_CullLights()
{
    shader_Use(rendererData.cullShader);
    
    shader_Dispatch(1, 1, 24);
}