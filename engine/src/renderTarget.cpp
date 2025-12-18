#include "engine/renderTarget.h"
#include <iostream>

RenderTarget render_target_Create(int width, int height)
{
    RenderTarget render_target{};
    render_target.width = width;
    render_target.height = height;

    glGenFramebuffers(1, &render_target.fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, render_target.fbo);

    glGenTextures(1, &render_target.colourTexture);
    glBindTexture(GL_TEXTURE_2D, render_target.colourTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, render_target.colourTexture, 0);

    glGenRenderbuffers(1, &render_target.depthRBO);
    glBindRenderbuffer(GL_RENDERBUFFER, render_target.depthRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, render_target.depthRBO);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cerr << "FBO is not complete!" << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return render_target;
}

void render_target_Resize(RenderTarget& render_target, int width, int height)
{
    if (width == render_target.width && height == render_target.height)
    {
        return;
    }

    render_target_Destroy(render_target);
    render_target = render_target_Create(width, height);
}

void render_target_Destroy(RenderTarget& render_target)
{
    if (render_target.depthRBO)
    {
        glDeleteRenderbuffers(1, &render_target.depthRBO);
    }

    if (render_target.colourTexture)
    {
        glDeleteTextures(1, &render_target.colourTexture);
    }

    if (render_target.fbo)
    {
        glDeleteFramebuffers(1, &render_target.fbo);
    }

    render_target = {};
}