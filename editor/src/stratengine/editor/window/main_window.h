#pragma once

#include <SDL.h>
#include <vulkan/vulkan.h>
#include <string>
#include "stratengine/macro.h"

STRATE_INNER_NAMESPACE(editor)
STRATE_MAKE_NAMESPACE(window)

class MainWindow {
public:
    MainWindow(const std::string& title, int width, int height);
    ~MainWindow();

    void Initialize();
    void Destroy();

    SDL_Window* GetSDLWindow() const;
    VkSurfaceKHR CreateVulkanSurface(VkInstance instance);

private:
    std::string title_;
    int width_;
    int height_;
    SDL_Window* window_;
    SDL_Surface* screen_surface_;
};

STRATE_END_NAMESPACE
STRATE_INNER_END_NAMESPACE