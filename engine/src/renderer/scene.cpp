#include "engine/scene.h"
#include "engine/shader.h"
#include <iostream>
#include <glad/glad.h>

Scene scene_Create()
{
    Scene scene = {};
    scene.modelCount = 0;

    for (int i = 0; i < MAX_SCENE_MODELS; ++i)
    {
        scene.models[i] = {};
    }

    return scene;
}

void scene_AddModel(Scene& scene, const Model& model)
{
    if (scene.modelCount >= MAX_SCENE_MODELS)
    {
        return;
    }

    scene.models[scene.modelCount] = model;
    scene.modelCount++;
}

void scene_Render(Scene& scene, RenderTarget& target)
{
    glBindFramebuffer(GL_FRAMEBUFFER, target.fbo);
    glViewport(0, 0, target.width, target.height);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (uint32_t i = 0; i < scene.modelCount; i++)
    {
        model_Draw(scene.models[i]);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void scene_Destroy(Scene& scene)
{
    for (uint32_t i = 0; i < scene.modelCount; i++)
    {
        model_Destroy(scene.models[i]);
    }

    scene.modelCount = 0;
}