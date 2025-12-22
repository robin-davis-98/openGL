#pragma once

#include "engine/engine.h"
#include "engine/window.h"
#include "engine/renderTarget.h"
#include "engine/scene.h"
#include <string>

struct ENGINE_API App
{
    uint32_t width;
    uint32_t height;
    std::string title;

    Window window;
    RenderTarget viewportRenderTarget;
    Scene scene;
};

ENGINE_API App app_Create(uint32_t width, uint32_t height, const std::string& title);

ENGINE_API bool app_Initialize(App& app);
ENGINE_API bool app_ShouldClose(App& app);
ENGINE_API void app_FixedUpdate(App& app);
ENGINE_API void app_Update(App& app);
ENGINE_API void app_Shutdown(App& app);