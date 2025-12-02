#include <engine/app.h>

int main()
{
    App app{800, 600, "Hello World"};

    if(!App_Initialize(app))
    {
        return -1;
    }
}