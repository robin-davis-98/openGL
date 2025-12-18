#include "engine/window.h"
#include "engine/app.h"
#include "engine/renderTarget.h"
#include <iostream>

static void framebuffer_size_callback(GLFWwindow* handle, int width, int height)
{
    App* app = static_cast<App*>(glfwGetWindowUserPointer(handle));
    if (!app) return;

    app->width  = width;
    app->height = height;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, width, height);
}

Window window_Create(uint32_t width, uint32_t height, const std::string& title)
{
    Window window{ width, height, title, nullptr};
    return window;
}

bool window_Initialize(Window& window)
{
    if(!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW!" << std::endl;
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window.nativeHandle = glfwCreateWindow(window.width, window.height, window.title.c_str(), nullptr, nullptr);
    if (!window.nativeHandle)
    {
        std::cerr << "Failed to create GLFW window!" << std::endl;
        return false;
    }

    glfwMakeContextCurrent(static_cast<GLFWwindow*>(window.nativeHandle));

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to intialize GLAD" << std::endl;
        glfwDestroyWindow(static_cast<GLFWwindow*>(window.nativeHandle));
        window.nativeHandle = nullptr;
        return false;
    }

    glfwSwapInterval(1);
    glfwSetFramebufferSizeCallback(window.nativeHandle, framebuffer_size_callback);
    
    return true;
}

bool window_ShouldClose(const Window& window)
{
    return glfwWindowShouldClose(window.nativeHandle);
}

void window_SwapBuffers(const Window& window)
{
    glfwSwapBuffers(window.nativeHandle);
}

void window_Update(Window& window)
{
    glfwPollEvents();
}

void window_Shutdown(Window& window)
{
    if (window.nativeHandle)
    {
        glfwDestroyWindow(window.nativeHandle);
    }
    glfwTerminate();
}

void window_Clear(const Window& window)
{
    glClearColor(
        window.clearColour.rgba.r,
        window.clearColour.rgba.g,
        window.clearColour.rgba.b,
        window.clearColour.rgba.a
    );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}