#include "stratengine/editor/window/main_window.h"
#include <SDL_vulkan.h>
#include <stdexcept>

STRATE_INNER_NAMESPACE(editor)
STRATE_MAKE_NAMESPACE(window)

MainWindow::MainWindow(const std::string& title, int width, int height)
                : title_(title),
                  width_(width),
                  height_(height),
                  window_(nullptr) {}

MainWindow::~MainWindow() {
  
}

void MainWindow::Initialize() {
  // Enable high-DPI support
  SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "0");

  // Create SDL window with Vulkan support and high-DPI support
  window_ = SDL_CreateWindow(
      title_.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width_,
      height_, SDL_WINDOW_VULKAN | SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
  if (!window_) {
    throw std::runtime_error("Failed to create SDL window");
  }
}

void MainWindow::Destroy() {
  if (window_) {
    SDL_DestroyWindow(window_);
    window_ = nullptr;
  }
  SDL_Quit();
}

SDL_Window* MainWindow::GetSDLWindow() const {
  return window_;
}

VkSurfaceKHR MainWindow::CreateVulkanSurface(VkInstance instance) {
  VkSurfaceKHR surface;
  if (!SDL_Vulkan_CreateSurface(window_, instance, &surface)) {
    throw std::runtime_error("Failed to create Vulkan surface");
  }
  return surface;
}

STRATE_END_NAMESPACE
STRATE_INNER_END_NAMESPACE