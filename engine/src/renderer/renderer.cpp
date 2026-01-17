#include "engine/renderer.h"
#include <glad/glad.h>

void renderer_Initialize()
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

void renderer_Shutdown()
{

}

void renderer_RenderViewport(App& app)
{
    RenderTarget& renderTarget = app.viewportRenderTarget;
    const Colour& clearColour = app.window.clearColour;

    glBindFramebuffer(GL_FRAMEBUFFER, renderTarget.fbo);
    glViewport(0, 0, renderTarget.width, renderTarget.height);

    glClearColor(
        clearColour.rgba.r,
        clearColour.rgba.g,
        clearColour.rgba.b,
        clearColour.rgba.a
    );

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (app.activeScene != nullptr)
    {
        scene_Render(*app.activeScene, app.viewportRenderTarget);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}