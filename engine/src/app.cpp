#include "engine/app.h"
#include "engine/renderer.h"
#include "engine/ubo.h"
#include "gui.h"
#include <iostream>

Shader outlineShader;

App app_Create(uint32_t width, uint32_t height, const std::string& title, const Scene* start)
{
    App app;
    app.width = width;
    app.height = height;
    app.title = title;
    app.startScene = (Scene*)start;
    app.activeScene = nullptr;
    app.lastFrame = 0.0f;
    app.deltaTime = 0.0f;

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
    renderer_Initialize();

    outlineShader = shader_Create("assets/shaders/default/outline.vert", "assets/shaders/default/outline.frag");
    ubo_BindToShader(app.cameraUBO, outlineShader, "CameraData");
    
    app.viewportRenderTarget = render_target_Create(app.width, app.height);

    return true;
}

void app_OnStart(App& app)
{
    app_LoadScene(app, app.startScene);
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
    float currentFrame = (float)glfwGetTime();
    app.deltaTime = currentFrame - app.lastFrame;
    app.lastFrame = currentFrame;

    if (app.activeScene && app.activeScene->currentCamera)
    {
        Camera& cam = *app.activeScene->currentCamera;

        if (app.viewportRenderTarget.height > 0) {
            cam.aspectRatio = (float)app.viewportRenderTarget.width / (float)app.viewportRenderTarget.height;
        }

        if (glfwGetInputMode(app.window.nativeHandle, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
        {
            if (glfwGetKey(app.window.nativeHandle, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            {
                glfwSetInputMode(app.window.nativeHandle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                cam.firstMouse = true;
            } else {

                camera_ProcessInput(cam, app, app.deltaTime);

                double xpos, ypos;
                glfwGetCursorPos(app.window.nativeHandle, &xpos, &ypos);
                camera_ProcessMouse(cam, (float)xpos, (float)ypos);
            }
        }

        CameraMatrices mats;
        mats.projection = camera_GetProjectionMatrix(cam);
        mats.view = camera_GetViewMatrix(cam);
        mats.nearPlane = cam.nearPlane;
        mats.farPlane = cam.farPlane;

        ubo_Update(app.cameraUBO, 0, sizeof(CameraMatrices), &mats);
    }

    renderer_RenderViewport(app);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, app.width, app.height);
    window_Clear(app.window);

    gui_Render(app);

    window_SwapBuffers(app.window);
    window_Update(app.window);
}

void app_Shutdown(App& app)
{
    render_target_Destroy(app.viewportRenderTarget);

    app.activeScene = nullptr;
    app.startScene = nullptr;

    window_Shutdown(app.window);
}

void app_LoadScene(App& app, const Scene* scene)
{
    app.activeScene = (Scene*)scene;
}