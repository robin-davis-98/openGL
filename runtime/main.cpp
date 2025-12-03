#include <engine/app.h>

int main()
{
    App app = app_Create(800, 600, "Hello, World!");

    if(!app_Initialize(app))
    {
        return -1;
    }

    while(!app_ShouldClose(app))
    {
        app_Update(app);
    }

    app_Shutdown(app);
}