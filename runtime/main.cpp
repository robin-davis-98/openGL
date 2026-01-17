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
        {{-0.5f, -0.5f, -0.5f},  {0.0f,  0.0f, -1.0f},  {0.0f, 0.0f}},
        {{ 0.5f, -0.5f, -0.5f},  {0.0f,  0.0f, -1.0f},  {1.0f, 0.0f}},
        {{ 0.5f,  0.5f, -0.5f},  {0.0f,  0.0f, -1.0f},  {1.0f, 1.0f}},
        {{-0.5f,  0.5f, -0.5f},  {0.0f,  0.0f, -1.0f},  {0.0f, 1.0f}},

        // Front face
        {{-0.5f, -0.5f,  0.5f},  {0.0f,  0.0f,  1.0f},  {0.0f, 0.0f}},
        {{ 0.5f, -0.5f,  0.5f},  {0.0f,  0.0f,  1.0f},  {1.0f, 0.0f}},
        {{ 0.5f,  0.5f,  0.5f},  {0.0f,  0.0f,  1.0f},  {1.0f, 1.0f}},
        {{-0.5f,  0.5f,  0.5f},  {0.0f,  0.0f,  1.0f},  {0.0f, 1.0f}},

        // Left face
        {{-0.5f,  0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f},  {1.0f, 0.0f}},
        {{-0.5f,  0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f},  {1.0f, 1.0f}},
        {{-0.5f, -0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f},  {0.0f, 1.0f}},
        {{-0.5f, -0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f},  {0.0f, 0.0f}},

        // Right face
        {{ 0.5f,  0.5f,  0.5f},  {1.0f,  0.0f,  0.0f},  {1.0f, 0.0f}},
        {{ 0.5f,  0.5f, -0.5f},  {1.0f,  0.0f,  0.0f},  {1.0f, 1.0f}},
        {{ 0.5f, -0.5f, -0.5f},  {1.0f,  0.0f,  0.0f},  {0.0f, 1.0f}},
        {{ 0.5f, -0.5f,  0.5f},  {1.0f,  0.0f,  0.0f},  {0.0f, 0.0f}},

        // Bottom face
        {{-0.5f, -0.5f, -0.5f},  {0.0f, -1.0f,  0.0f},  {0.0f, 1.0f}},
        {{ 0.5f, -0.5f, -0.5f},  {0.0f, -1.0f,  0.0f},  {1.0f, 1.0f}},
        {{ 0.5f, -0.5f,  0.5f},  {0.0f, -1.0f,  0.0f},  {1.0f, 0.0f}},
        {{-0.5f, -0.5f,  0.5f},  {0.0f, -1.0f,  0.0f},  {0.0f, 0.0f}},

        // Top face
        {{-0.5f,  0.5f, -0.5f},  {0.0f,  1.0f,  0.0f},  {0.0f, 1.0f}},
        {{ 0.5f,  0.5f, -0.5f},  {0.0f,  1.0f,  0.0f},  {1.0f, 1.0f}},
        {{ 0.5f,  0.5f,  0.5f},  {0.0f,  1.0f,  0.0f},  {1.0f, 0.0f}},
        {{-0.5f,  0.5f,  0.5f},  {0.0f,  1.0f,  0.0f},  {0.0f, 0.0f}},
    };

    uint32_t cubeIndices[] = {
        0,  1,  2,  2,  3,  0,  // Back
        4,  5,  6,  6,  7,  4,  // Front
        8,  9,  10, 10, 11, 8,  // Left
        12, 13, 14, 14, 15, 12, // Right
        16, 17, 18, 18, 19, 16, // Bottom
        20, 21, 22, 22, 23, 20  // Top
    };

    uint32_t vertexCount = sizeof(cube) / sizeof(Vertex);
    uint32_t indexCount = sizeof(cubeIndices) / sizeof(uint32_t);

    Model triangleModel = model_Create(cube, vertexCount, cubeIndices, indexCount, basicMaterial);
    Model triangleModelBlue = model_Create(cube, vertexCount, cubeIndices, indexCount, blueMaterial);
    Model triangleModelGreen = model_Create(cube, vertexCount, cubeIndices, indexCount, greenMaterial);

    scene_AddModel(blue, triangleModelBlue);
    scene_AddModel(green, triangleModelGreen);

    triangleModelBlue.position = glm::vec3(5.0f, 0.0f, 0.0f);

    scene_AddModel(red, triangleModel);
    scene_AddModel(red, triangleModelBlue);

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