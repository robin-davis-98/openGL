#pragma once

#include "engine/engine.h"
#include <string>

struct ENGINE_API App
{
    uint32_t width;
    uint32_t height;
    std::string title;
};

ENGINE_API bool App_Initialize(App& app);
ENGINE_API void App_Update(App& app);
ENGINE_API void App_Shutdown(App& app);