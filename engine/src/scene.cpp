#include "engine/scene.h"
#include "engine/shader.h"
#include <glad/glad.h>


static float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};

void scene_Initialize(Scene& scene)
{
    scene.shader = shader_Create("assets/shaders/basic.vert", "assets/shaders/basic.frag");

    glGenVertexArrays(1, &scene.vertexArray);
    glGenBuffers(1, &scene.vertexBuffer);

    glBindVertexArray(scene.vertexArray);

    glBindBuffer(GL_ARRAY_BUFFER, scene.vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void scene_Render(Scene& scene, RenderTarget& target)
{
    glBindFramebuffer(GL_FRAMEBUFFER, target.fbo);
    glViewport(0, 0, target.width, target.height);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader_Use(scene.shader);
    glBindVertexArray(scene.vertexArray);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void scene_Destroy(Scene& scene)
{
    shader_Destroy(scene.shader);

    if (scene.vertexArray != 0)
    {
        glDeleteVertexArrays(1, &scene.vertexArray);
        scene.vertexArray = 0;
    }

    if (scene.vertexBuffer != 0)
    {
        glDeleteBuffers(1, &scene.vertexBuffer);
        scene.vertexBuffer = 0;
    }
}