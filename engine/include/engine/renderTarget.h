#pragma once
#include <glad/glad.h>
#include <cstdint>

struct RenderTarget
{
    GLuint fbo = 0;
    GLuint colourTexture = 0;
    GLuint depthRBO = 0;
    uint32_t width = 0;
    uint32_t height = 0;
};

RenderTarget render_target_Create(int width, int height);
void render_target_Resize(RenderTarget& render_target, int width, int height);
void render_target_Destroy(RenderTarget& render_target);