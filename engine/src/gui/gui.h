#pragma once
#include "engine/engine.h"
#include "engine/window.h"
#include "engine/renderTarget.h"
#include "engine/app.h"

void gui_Initialize(Window& window);
void gui_NewFrame(App& app);
void gui_Render();
void gui_Shutdown();
void gui_RenderViewport(App& app);
