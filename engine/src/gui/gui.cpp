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
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

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

    ImGui::PopStyleVar();
}

void gui_RenderMainMenuBar(App& app)
{
    if (ImGui::BeginMainMenuBar())
    {
        if(ImGui::BeginMenu("System"))
        {
            if(ImGui::MenuItem("Exit", "Alt+F4"))
            {
                glfwSetWindowShouldClose(app.window.nativeHandle, true);
            }

            ImGui::EndMenu();
        }

        if(ImGui::BeginMenu("Scenes"))
        {
            if(ImGui::MenuItem("Save")){}
            if(ImGui::MenuItem("Load")){}
            
            ImGui::Separator();

            if(ImGui::MenuItem("Scene 1")) { app.sceneChange = 0; }
            if(ImGui::MenuItem("Scene 2")) { app.sceneChange = 1; }
            if(ImGui::MenuItem("Scene 3")) { app.sceneChange = 2; }

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}

void gui_DrawNodeTree(Node* node)
{
    if (!node) return;

    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
    if (node->children.empty()) flags |= ImGuiTreeNodeFlags_Leaf;
    if (selectedNode == node) flags |= ImGuiTreeNodeFlags_Selected;

    bool opened = ImGui::TreeNodeEx((void*)node, flags, "%s", node->name.c_str());

    if (ImGui::IsItemClicked())
    {
        selectedNode = node;
    }

    if (opened)
    {
        for (Node* child : node->children)
        {
            gui_DrawNodeTree(child);
        }
        ImGui::TreePop();
    }
}

void gui_RenderHierarchy(App& app)
{
    ImGui::Begin("Scene Hierarchy");

    if (app.activeScene && app.activeScene->root)
    {
        for (Node* child : app.activeScene->root->children)
        {
            gui_DrawNodeTree(child);
        }
    }

    ImGui::End();
}

void gui_RenderRuntime(App& app)
{
    ImGui::Begin("Runtime Statistics");
   ImGuiIO& io = ImGui::GetIO();

    // Display FPS
    ImGui::Text("Frames Per Second: %.1f FPS", io.Framerate);
    
    // Display Frame Time (ms)
    // 1000.0f / io.Framerate gives the average ms per frame
    ImGui::Text("Frame Time: %.3f ms", 1000.0f / io.Framerate);

    ImGui::Separator();

    // Visual Sparkline (Optional but cool for a debug UI)
    static float values[90] = { 0 };
    static int values_offset = 0;
    static double refresh_time = 0.0;
    
    if (refresh_time == 0.0) refresh_time = ImGui::GetTime();
    
    // Update the graph every 1/60th of a second
    while (refresh_time < ImGui::GetTime()) {
        values[values_offset] = 1000.0f / io.Framerate;
        values_offset = (values_offset + 1) % IM_ARRAYSIZE(values);
        refresh_time += 1.0f / 60.0f;
    }

    float max_ms = 33.33f; // Scale graph to 30fps baseline
    ImGui::PlotLines("ms/frame", values, IM_ARRAYSIZE(values), values_offset, nullptr, 0.0f, max_ms, ImVec2(0, 80));
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
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    ImGui::Begin("Dockspace Window", nullptr, window_flags);
    ImGui::PopStyleVar(3);

    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0, 0), dockspace_flags);
    ImGui::End();

    gui_RenderMainMenuBar(app);
    gui_RenderViewport(app);
    gui_RenderHierarchy(app);
    gui_RenderRuntime(app);

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

void gui_Render(App& app)
{
    gui_NewFrame(app);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void gui_Shutdown()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}