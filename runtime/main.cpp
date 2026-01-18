#include <engine/app.h>
#include <engine/model.h>
#include <engine/scene.h>
#include <engine/ubo.h>
#include <engine/camera.h>
#include <iostream>

int main()
{
    Scene red = scene_Create();
    Scene blue = scene_Create();
    Scene green = scene_Create();

    App app = app_Create(800, 600, "Model Viewer", &red);

    if(!app_Initialize(app))
    {
        return -1;
    }

    Shader basicShader = shader_Create("assets/shaders/basic.vert", "assets/shaders/basic.frag");
    Material basicMaterial = material_Create(&basicShader);
    
    Shader blueShader = shader_Create("assets/shaders/basic.vert", "assets/shaders/blue.frag");
    Material blueMaterial = material_Create(&blueShader);

    Shader greenShader = shader_Create("assets/shaders/basic.vert", "assets/shaders/green.frag");
    Material greenMaterial = material_Create(&greenShader);

    UniformBuffer cameraUBO = ubo_Create(sizeof(CameraMatrices), 0);
    app.cameraUBO = cameraUBO;

    ubo_BindToShader(cameraUBO, basicShader, "CameraData");
    ubo_BindToShader(cameraUBO, blueShader, "CameraData");
    ubo_BindToShader(cameraUBO, greenShader, "CameraData");

    Vertex cube[] = {
        // Back face (Z = -0.5)
        {{-0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {0.0f, 0.0f}}, // 0
        {{ 0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {1.0f, 0.0f}}, // 1
        {{ 0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {1.0f, 1.0f}}, // 2
        {{-0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {0.0f, 1.0f}}, // 3

        // Front face (Z = 0.5)
        {{-0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {0.0f, 0.0f}}, // 4
        {{ 0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {1.0f, 0.0f}}, // 5
        {{ 0.5f,  0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {1.0f, 1.0f}}, // 6
        {{-0.5f,  0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {0.0f, 1.0f}}, // 7

        // Left face (X = -0.5)
        {{-0.5f,  0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 0.0f}}, // 8
        {{-0.5f,  0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 1.0f}}, // 9
        {{-0.5f, -0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, {0.0f, 1.0f}}, // 10
        {{-0.5f, -0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, {0.0f, 0.0f}}, // 11

        // Right face (X = 0.5)
        {{ 0.5f,  0.5f,  0.5f}, { 1.0f,  0.0f,  0.0f}, {1.0f, 0.0f}}, // 12
        {{ 0.5f,  0.5f, -0.5f}, { 1.0f,  0.0f,  0.0f}, {1.0f, 1.0f}}, // 13
        {{ 0.5f, -0.5f, -0.5f}, { 1.0f,  0.0f,  0.0f}, {0.0f, 1.0f}}, // 14
        {{ 0.5f, -0.5f,  0.5f}, { 1.0f,  0.0f,  0.0f}, {0.0f, 0.0f}}, // 15

        // Bottom face (Y = -0.5)
        {{-0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f,  0.0f}, {0.0f, 1.0f}}, // 16
        {{ 0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f,  0.0f}, {1.0f, 1.0f}}, // 17
        {{ 0.5f, -0.5f,  0.5f}, { 0.0f, -1.0f,  0.0f}, {1.0f, 0.0f}}, // 18
        {{-0.5f, -0.5f,  0.5f}, { 0.0f, -1.0f,  0.0f}, {0.0f, 0.0f}}, // 19

        // Top face (Y = 0.5)
        {{-0.5f,  0.5f, -0.5f}, { 0.0f,  1.0f,  0.0f}, {0.0f, 1.0f}}, // 20
        {{ 0.5f,  0.5f, -0.5f}, { 0.0f,  1.0f,  0.0f}, {1.0f, 1.0f}}, // 21
        {{ 0.5f,  0.5f,  0.5f}, { 0.0f,  1.0f,  0.0f}, {1.0f, 0.0f}}, // 22
        {{-0.5f,  0.5f,  0.5f}, { 0.0f,  1.0f,  0.0f}, {0.0f, 0.0f}}, // 23
    };

    uint32_t cubeIndices[] = {
        0,  2,  1,  0,  3,  2,  // Back (Clockwise corrected for CCW view)
        4,  5,  6,  4,  6,  7,  // Front
        8,  9,  10, 8,  10, 11, // Left
        12, 14, 13, 12, 15, 14, // Right
        16, 17, 18, 16, 18, 19, // Bottom
        20, 22, 21, 20, 23, 22  // Top
    };

    uint32_t vertexCount = sizeof(cube) / sizeof(Vertex);
    uint32_t indexCount = sizeof(cubeIndices) / sizeof(uint32_t);

    Model triangleModel = model_Create(cube, vertexCount, cubeIndices, indexCount, basicMaterial);
    Model triangleModelBlue = model_Create(cube, vertexCount, cubeIndices, indexCount, blueMaterial);
    Model triangleModelGreen = model_Create(cube, vertexCount, cubeIndices, indexCount, greenMaterial);

    scene_AddModel(blue, nullptr, &triangleModelBlue, "BlueCube");
    scene_AddModel(green, nullptr, &triangleModelGreen, "GreenCube");

    Node* Cube1 = scene_AddModel(red, nullptr, &triangleModel, "Cube");
    Node* redCube2 = scene_AddModel(red, nullptr, &triangleModelBlue, "BlueCube");

    redCube2->position = glm::vec3(5.0f, 0.0f, 0.0f);
    Cube1->rotation.y = 30.0f;

    shader_SetVec3(basicShader, "uBaseColour", {1.0f, 1.0f, 1.0f});

    Node* lightNode = scene_AddPointLight(red, nullptr, glm::vec3(1, 1, 1), 2.0f, 20.0f, "Light");

    lightNode->position = glm::vec3(2.0f, 2.0f, 2.0f);

    Node* lightNode2 = scene_AddPointLight(red, redCube2, glm::vec3(0, 0, 1), 5.0f, 5.0f, "Light");

    Camera mainCam = {};
    mainCam.position    = glm::vec3(0.0f, 0.0f, 5.0f);
    mainCam.front       = glm::vec3(0.0f, 0.0f, -1.0f);
    mainCam.up          = glm::vec3(0.0f, 1.0f, 0.0f);
    mainCam.yaw         = -90.0f; // Essential: looks down the -Z axis
    mainCam.pitch       = 0.0f;
    mainCam.fov         = 45.0f;
    mainCam.aspectRatio = (float)app.viewportRenderTarget.width / (float)app.viewportRenderTarget.height;
    mainCam.nearPlane   = 0.1f;
    mainCam.farPlane    = 100.0f;
    mainCam.firstMouse  = true;

    red.currentCamera = &mainCam;
    blue.currentCamera = &mainCam;
    green.currentCamera = &mainCam;

    app_OnStart(app);

    while(!app_ShouldClose(app))
    {
        app_Update(app);

        if (app.sceneChange != -1)
        {
            if (app.sceneChange == 0)
            {
                app_LoadScene(app, &red);
            }

            if (app.sceneChange == 1)
            {
                app_LoadScene(app, &blue);
            }

            if (app.sceneChange == 2)
            {
                app_LoadScene(app, &green);
            }

            app.sceneChange = -1;
        }
    }

    scene_Destroy(red);
    scene_Destroy(blue);
    scene_Destroy(green);
    app_Shutdown(app);
}