#pragma once
#include "engine/engine.h"
#include "engine/window.h"
#include "engine/renderTarget.h"

void gui_Initialize(Window& window);
void gui_NewFrame();
void gui_Render();
void gui_Shutdown();
void gui_RenderViewport(RenderTarget& render_target);
