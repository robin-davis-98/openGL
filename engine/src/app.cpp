#include "engine/app.h"
#include "engine/renderer.h"
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

    glfwSetWindowUserPointer(app.window.nativeHandle, &app);
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
    renderer_RenderViewport(app);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, app.width, app.height);

    window_Clear(app.window);

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