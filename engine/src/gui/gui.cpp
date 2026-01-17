#include "gui.h"
#include <iostream>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui_internal.h>

static bool dockspace_initialized = false;
static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

void gui_Initialize(Window& window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui_ImplGlfw_InitForOpenGL(window.nativeHandle, true);
    ImGui_ImplOpenGL3_Init("#version 430");
}

void gui_RenderViewport(App& app)
{
    ImGui::Begin("Viewport");

    ImVec2 size = ImGui::GetContentRegionAvail();

    // Resize FBO if needed
    if (size.x > 0 && size.y > 0)
    {
        if ((int)size.x != app.viewportRenderTarget.width ||
            (int)size.y != app.viewportRenderTarget.height)
        {
            render_target_Resize(
                app.viewportRenderTarget,
                (int)size.x,
                (int)size.y
            );
        }
    }

    // ALWAYS draw the viewport image
    ImGui::Image(
        (void*)(intptr_t)app.viewportRenderTarget.colourTexture,
        size,
        ImVec2(0, 1),
        ImVec2(1, 0)
    );

    if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
        {
            glfwSetInputMode(app.window.nativeHandle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            if (app.activeScene && app.activeScene->currentCamera)
            {
                app.activeScene->currentCamera->firstMouse = true;
            }
        }
        ImGui::End();
    }


void gui_NewFrame(App& app)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    const ImGuiViewport* viewport = ImGui::GetMainViewport();

    // Dockspace window -----------------------------------------------------
    ImGuiWindowFlags window_flags =
        ImGuiWindowFlags_NoDocking |
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoBringToFrontOnFocus |
        ImGuiWindowFlags_NoNavFocus;

    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::Begin("Dockspace Window", nullptr, window_flags);
    ImGui::PopStyleVar(2);

    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0, 0), dockspace_flags);
    ImGui::End();

    ImGui::Begin("Scene Hierarchy");

    if (ImGui::Button("Scene 1", ImVec2(-1, 0)))
    {
        app.sceneChange = 0;
    }

    ImGui::Separator();

    if (ImGui::Button("Scene 2", ImVec2(-1, 0)))
    {
        app.sceneChange = 1;
    }

    ImGui::Separator();

    if (ImGui::Button("Scene 3", ImVec2(-1, 0)))
    {
        app.sceneChange = 2;
    }

    ImGui::End();

    ImGui::Begin("Runtime Statistics");
    ImGui::Text("Runtime");
    ImGui::End();

    if (!dockspace_initialized)
    {
        ImGui::DockBuilderRemoveNode(dockspace_id);
        ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
        ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->WorkSize);

        ImGuiID left, right;
        ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.25f, &left, &right);

        ImGuiID left_top, left_bottom;
        ImGui::DockBuilderSplitNode(left, ImGuiDir_Up, 0.8f, &left_top, &left_bottom);

        ImGui::DockBuilderDockWindow("Scene Hierarchy", left_top);
        ImGui::DockBuilderDockWindow("Runtime Statistics", left_bottom);
        ImGui::DockBuilderDockWindow("Viewport", right);

        ImGui::DockBuilderFinish(dockspace_id);

        dockspace_initialized = true;
    }
}

void gui_Render()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void gui_Shutdown()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}