#pragma once
#include "engine/engine.h"
#include "engine/colour.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

struct Window
{
    uint32_t width{};
    uint32_t height{};
    std::string title;
    GLFWwindow* nativeHandle{nullptr};
    Colour clearColour{ {0.247f, 0.271f, 0.243f, 1.0f} };
};

Window window_Create(uint32_t width, uint32_t height, const std::string& title);

bool window_Initialize(Window& window);
bool window_ShouldClose(const Window& window);
void window_Update(Window& window);
void window_SwapBuffers(const Window& window);
void window_Shutdown(Window& window);
void window_Clear(const Window& window);