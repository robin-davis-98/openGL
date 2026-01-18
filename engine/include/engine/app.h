#pragma once

#include "engine/engine.h"
#include "engine/window.h"
#include "engine/renderTarget.h"
#include "engine/scene.h"
#include "engine/camera.h"
#include "engine/shader.h"
#include "engine/ubo.h"
#include <string>

struct ENGINE_API App
{
    uint32_t width;
    uint32_t height;
    std::string title;
    float lastFrame;
    float deltaTime;

    Camera currentCamera;
    UniformBuffer cameraUBO;

    Window window;
    RenderTarget viewportRenderTarget;

    Scene* startScene;
    Scene* activeScene;
    uint32_t sceneChange = -1;
};

extern Shader outlineShader;

ENGINE_API App app_Create(uint32_t width, uint32_t height, const std::string& title, const Scene* start);

ENGINE_API bool app_Initialize(App& app);
ENGINE_API void app_OnStart(App& app);
ENGINE_API bool app_ShouldClose(App& app);
ENGINE_API void app_FixedUpdate(App& app);
ENGINE_API void app_Update(App& app);
ENGINE_API void app_Shutdown(App& app);

ENGINE_API void app_LoadScene(App& app, const Scene* scene);