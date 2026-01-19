#include "engine/renderer.h"
#include <glad/glad.h>
#include <iostream>

RendererData rendererData = {};
float playerHealth = 0.75f;

void renderer_Initialize()
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);

    rendererData.gridSize = glm::uvec3(16, 9, 24);
    rendererData.clusterCount = rendererData.gridSize.x * rendererData.gridSize.y * rendererData.gridSize.z;

    rendererData.clusterBuildShader = shader_CreateCompute("assets/shaders/default/cluster_build.comp");
    rendererData.cullShader = shader_CreateCompute("assets/shaders/default/cluster_cull.comp");

    renderer_InitializeSSBO();
    renderer_InitUI();
}

void renderer_Shutdown()
{

}

void renderer_RenderViewport(App& app)
{
    if (app.activeScene != nullptr)
    {
        scene_Render(*app.activeScene, app.viewportRenderTarget, app.window.clearColour, app.cameraUBO.ID);
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

    glDispatchCompute(1, 1, rendererData.gridSize.z);

    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

void renderer_CullLights()
{
    shader_Use(rendererData.cullShader);
    
    shader_Dispatch(1, 1, 24);
}

void renderer_BuildClusters(float screenWidth, float screenHeight, uint32_t& cameraUBO)
{
    if (rendererData.currentResolution.x == screenWidth && rendererData.currentResolution.y == screenHeight) return;

    rendererData.currentResolution = glm::vec2(screenWidth, screenHeight);

    shader_Use(rendererData.clusterBuildShader);

    glBindBufferBase(GL_UNIFORM_BUFFER, 0, cameraUBO);

    shader_SetUI3(rendererData.clusterBuildShader, "u_GridSize", 
        rendererData.gridSize.x, rendererData.gridSize.y, rendererData.gridSize.z);
    shader_SetVec2(rendererData.clusterBuildShader, "u_ScreenSize", rendererData.currentResolution);

    shader_Dispatch(rendererData.gridSize.x, rendererData.gridSize.y, rendererData.gridSize.z);
}

void renderer_checkBufferData()
{
    glFinish(); 

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, rendererData.clusterSSBO);
    
    // 2. Map the buffer
    Cluster* ptr = (Cluster*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
    
    if (ptr) {
        std::cout << "--- Cluster SSBO Debug ---" << std::endl;
        // Check the very first cluster (usually bottom-left-near)
        std::cout << "Cluster[0] Min: " << ptr[0].minPoint.x << ", " << ptr[0].minPoint.y << ", " << ptr[0].minPoint.z << std::endl;
        std::cout << "Cluster[0] Max: " << ptr[0].maxPoint.x << ", " << ptr[0].maxPoint.y << ", " << ptr[0].maxPoint.z << std::endl;
        
        // Check a cluster in the middle of the grid
        int mid = rendererData.clusterCount / 2;
        std::cout << "Cluster[" << mid << "] Min: " << ptr[mid].minPoint.z << std::endl;
        
        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
    } else {
        std::cerr << "Failed to map Cluster SSBO!" << std::endl;
    }
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void renderer_InitUI()
{
    rendererData.uiData.shader = shader_Create("assets/shaders/default/ui.vert", "assets/shaders/default/ui.frag");

    glGenVertexArrays(1, &rendererData.uiData.vao);
    glGenBuffers(1, &rendererData.uiData.vbo);

    glBindVertexArray(rendererData.uiData.vao);
    glBindBuffer(GL_ARRAY_BUFFER, rendererData.uiData.vbo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(UIVertex) * 6 * 1000, nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(UIVertex), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(UIVertex), (void*)offsetof(UIVertex, uv));
}

void ui_DrawRect(glm::vec2 pos, glm::vec2 size, glm::vec4 colour, uint32_t textureID)
{
    UIVertex vertices[6] = {
        { pos, {0, 1} }, { pos + glm::vec2(0, size.y), {0, 0} }, { pos + size, {1, 0} },
        { pos, {0, 1} }, { pos + size, {1, 0} }, { pos + glm::vec2(size.x, 0), {1, 1} }
    };

    shader_Use(rendererData.uiData.shader);

    shader_SetVec4(rendererData.uiData.shader, "u_Colour", colour);

    if (textureID != 0)
    {
        shader_SetBool(rendererData.uiData.shader, "u_HasTexture", true);
        glBindTexture(GL_TEXTURE_2D, textureID);
    } else {
        shader_SetBool(rendererData.uiData.shader, "u_HasTexture", false);
    }

    glBindBuffer(GL_ARRAY_BUFFER, rendererData.uiData.vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

    glBindVertexArray(rendererData.uiData.vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
} 

glm::vec4 ui_GetHealthColour(float percent)
{
    glm::vec4 red = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    glm::vec4 yellow = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
    glm::vec4 green = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);

    if (percent > 0.5f)
    {
        float factor = (percent - 0.5f) * 2.0f;
        return glm::mix(yellow, green, factor);
    } else {
        float factor = percent * 2.0f;
        return glm::mix(red, yellow, factor);
    }
}

void ui_DrawHealthBar(glm::vec2 position, glm::vec2 size, float healthPercent)
{
    ui_DrawRect(position, size, glm::vec4(0.1f, 0.1f, 0.1f, 0.8f));

    glm::vec4 barColour = ui_GetHealthColour(healthPercent);

    glm::vec2 fillSize = glm::vec2(size.x * healthPercent, size.y);
    ui_DrawRect(position, fillSize, barColour);
}

bool ui_IsPointInRect(glm::vec2 point, glm::vec2 pos, glm::vec2 size)
{
    return (point.x >= pos.x && point.x <= pos.x + size.x &&
            point.y >= pos.y && point.y <= pos.y + size.y);
}

void renderer_BeginUI(int width, int height)
{
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    shader_Use(rendererData.uiData.shader);

    rendererData.uiData.projection = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);

    shader_SetMat4(rendererData.uiData.shader, "u_OrthoProjection", rendererData.uiData.projection);
}

void renderer_EndUI()
{
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glEnable(GL_CULL_FACE);
}