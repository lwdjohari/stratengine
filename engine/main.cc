#include <SDL.h>
#include <SDL_syswm.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <cstring>
#include <iostream>
#include "stratengine/stratengine.h"

int main(int argc, char *argv[])
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_Window *window = SDL_CreateWindow("RTS Game Engine",
                                          SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                          1280, 720, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (!window)
    {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    // Get SDL window handle for bgfx
    SDL_SysWMinfo wmi;
    SDL_VERSION(&wmi.version);
    if (!SDL_GetWindowWMInfo(window, &wmi))
    {
        std::cerr << "SDL_GetWindowWMInfo Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Set platform data
    bgfx::PlatformData pd;
    memset(&pd, 0, sizeof(pd));

    void *nativeWindowHandle = nullptr;
#if defined(_WIN32) || defined(_WIN64)
    nativeWindowHandle = (void *)wmi.info.win.window;
    std::cout << "Windows native window handle obtained." << std::endl;
#elif defined(__APPLE__)
#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
    nativeWindowHandle = wmi.info.uikit.window;
    std::cout << "iOS native window handle obtained." << std::endl;
#elif TARGET_OS_MAC
    nativeWindowHandle = (void *)wmi.info.cocoa.window;
    std::cout << "macOS native window handle obtained." << std::endl;
#endif
#elif defined(__ANDROID__)
    nativeWindowHandle = (void *)wmi.info.android.window;
    std::cout << "Android native window handle obtained." << std::endl;
#elif defined(__linux__)
    if (wmi.subsystem == SDL_SYSWM_X11)
    {
        nativeWindowHandle = (void *)(uintptr_t)wmi.info.x11.window;
        pd.ndt = wmi.info.x11.display;
        std::cout << "Linux (X11) native window handle obtained." << std::endl;
    }
    else
    {
        std::cerr << "Unsupported Linux window manager subsystem!" << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }
#else
    std::cerr << "Unsupported platform!" << std::endl;
    SDL_DestroyWindow(window);
    SDL_Quit();
    return -1;
#endif

    if (nativeWindowHandle == nullptr)
    {
        std::cerr << "Failed to get native window handle!" << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    pd.nwh = nativeWindowHandle;
    pd.context = nullptr;      // No context needed
    pd.backBuffer = nullptr;   // No backBuffer needed
    pd.backBufferDS = nullptr; // No backBufferDS needed
    bgfx::setPlatformData(pd);

    // Initialize bgfx
    bgfx::renderFrame();
    bgfx::Init bgfxInit;
    bgfxInit.type = bgfx::RendererType::OpenGL; // Automatically choose renderer
    bgfxInit.resolution.width = 1280;
    bgfxInit.resolution.height = 720;
    bgfxInit.resolution.reset = BGFX_RESET_VSYNC;
    if (!bgfx::init(bgfxInit))
    {
        std::cerr << "bgfx::init failed!" << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    std::cout << "bgfx initialized successfully." << std::endl;

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
            }
        }

        // Clear the screen and draw
        // bgfx::touch(0);
        // bgfx::frame();
    }

    // Shutdown bgfx
    bgfx::shutdown();

    // Cleanup SDL
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
