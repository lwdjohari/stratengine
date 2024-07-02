#pragma once
#include <SDL2/SDL.h>
#include "stratengine/macro.h"
#include "stratengine/editor/ui/viewport_manager.h"
#include "stratengine/editor/renderer/vulkan_context.h"
#include "stratengine/editor/renderer/imgui_context.h"

STRATE_INNER_NAMESPACE(editor)

class Application {
 public:
  Application();
  ~Application();

 private:
  void MainLoop();

  SDL_Window* window_;
  renderer::VulkanContext vulkan_context_;
  renderer::ImGuiContext imgui_context_;
  bool running_;
  ui::ViewportManager viewports_;
};

STRATE_INNER_END_NAMESPACE