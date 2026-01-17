#include <engine/app.h>
#include <engine/model.h>
#include <engine/scene.h>
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

    Vertex triangle[] = {
        { -0.5f, -0.5f, 0.0f },
        {  0.5f, -0.5f, 0.0f },
        {  0.0f,  0.5f, 0.0f }
    };

    uint32_t count = sizeof(triangle) / sizeof(Vertex);

    Model triangleModel = model_Create(triangle, count, basicMaterial);
    Model triangleModelBlue = model_Create(triangle, count, blueMaterial);
    Model triangleModelGreen = model_Create(triangle, count, greenMaterial);

    scene_AddModel(red, triangleModel);
    scene_AddModel(blue, triangleModelBlue);
    scene_AddModel(green, triangleModelGreen);

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