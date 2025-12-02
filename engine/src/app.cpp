#include "engine/app.h"
#include <iostream>

bool app_Initialize(App& app)
{
    std::cout << "Hello World, From the Engine!" << std::endl;

    return true;
}

bool app_ShouldClose(App& app)
{
    return false;
}

void app_Update(App& app)
{

}

void app_Shutdown(App& app)
{

}