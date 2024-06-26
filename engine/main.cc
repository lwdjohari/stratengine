#include <SDL.h>
#include <SDL_syswm.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

#include <cstring>
#include <iostream>

#include "stratengine/stratengine.h"

int main(int argc, char* argv[]) {
  using namespace stratengine;

  platform::StratEngineApp app;
  platform::RenderedBackend rendered_backend;
  platform::LinuxDisplayType linux_display_type;

// Auto Determine what best rendered in linux, windows, osx, android & ios and
// the fallback
#if STRATE_PLATFORM_LINUX == 1
  rendered_backend = platform::DetermineRenderedBackend(
      platform::RenderedBackend::kVulkan, platform::RenderedBackend::kOpenGL);

  linux_display_type = platform::DetermineLinuxDisplayType(
      platform::LinuxDisplayType::kWayland, platform::LinuxDisplayType::kX11);

#endif

#if STRATE_PLATFORM_WINDOWS == 1
  rendered_backend = platform::DetermineRenderedBackend(
      platform::RenderedBackend::kDirect3D12,
      platform::RenderedBackend::kDirect3D11);

  if (rendered_backend == platform::RenderedBackend::kNone)
    rendered_backend = platform::DetermineRenderedBackend(
        platform::RenderedBackend::kDirect3D11,
        platform::RenderedBackend::kOpenGL);

#endif

#if STRATE_PLATFORM_ANDROID == 1
  rendered_backend = platform::DetermineRenderedBackend(
      platform::RenderedBackend::kOpenGLES, platform::RenderedBackend::kOpenGL);
#endif

  app.InitApp(rendered_backend, "Hello-Stratengine",
              linux_display_type == platform::LinuxDisplayType::kWayland);

  app.InitRenderedBackend();

  // Main loop
  bool running = true;
  SDL_Event event;
  while (running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        running = false;
        break;
      }
    }

    if (!running)
      break;
    // Clear the screen and draw
    bgfx::touch(0);
    bgfx::frame();
  }

  std::cout << "Rendered Backend:   "
            << platform::ToStringEnumRenderedBackend(rendered_backend)
            << std::endl;
  std::cout << "Linux Display:      "
            << platform::ToStringEnumLinuxDisplayType(linux_display_type)
            << std::endl;

  app.ShutdownRenderedBackend();
  app.ShutdownApp();

  return 0;
}
