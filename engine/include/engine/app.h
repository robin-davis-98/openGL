#pragma once

#include "engine/engine.h"
#include <string>

struct ENGINE_API App
{
    uint32_t width;
    uint32_t height;
    std::string title;
};

ENGINE_API bool app_Initialize(App& app);
ENGINE_API bool app_ShouldClose(App& app);
ENGINE_API void app_Update(App& app);
ENGINE_API void app_Shutdown(App& app);