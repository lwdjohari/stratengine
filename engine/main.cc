#include <SDL.h>
#include <SDL_syswm.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <cstring>
#include <iostream>
#include "stratengine/stratengine.h"

int main(int argc, char *argv[])
{
    using namespace stratengine;

    platform::StratEngineApp app;

    app.InitApp(platform::RenderedBackend::kVulkan,"Hello-Stratengine");
    app.InitRenderedBackend();

    // Main loop
    bool running = true;
    SDL_Event event;
    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
                break;
            }
        }

        if(!running)
            break;
        //Clear the screen and draw
        bgfx::touch(0);
        bgfx::frame();
    }

    app.ShutdownRenderedBackend();
    app.ShutdownApp();

    return 0;
}
