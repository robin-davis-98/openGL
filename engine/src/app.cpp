#include "engine/app.h"
#include "gui.h"
#include <iostream>

App app_Create(uint32_t width, uint32_t height, const std::string& title)
{
    App app;
    app.width = width;
    app.height = height;
    app.title = title;

    return app;
}

bool app_Initialize(App& app)
{
    app.window = window_Create(app.width, app.height, app.title);

    if (!window_Initialize(app.window))
    {
        std::cerr << "Failed to initalize window!" << std::endl;
        return false;
    }

    gui_Initialize(app.window);

    app.viewportRenderTarget = render_target_Create(app.width, app.height);

    return true;
}

bool app_ShouldClose(App& app)
{
    return window_ShouldClose(app.window);
}

void app_FixedUpdate(App& app)
{

}

void app_Update(App& app)
{
    glBindFramebuffer(GL_FRAMEBUFFER, app.viewportRenderTarget.fbo);
    glViewport(0, 0, app.viewportRenderTarget.width, app.viewportRenderTarget.height);

    glClearColor(
        app.window.clearColour.rgba.r,
        app.window.clearColour.rgba.g,
        app.window.clearColour.rgba.b,
        app.window.clearColour.rgba.a
    );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    gui_NewFrame();
    gui_RenderViewport(app.viewportRenderTarget);
    gui_Render();

    window_SwapBuffers(app.window);
    window_Update(app.window);
}

void app_Shutdown(App& app)
{
    render_target_Destroy(app.viewportRenderTarget);
    window_Shutdown(app.window);
}