#pragma once
#include <SDL.h>
#include <SDL_syswm.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

#include <iostream>

#if BX_PLATFORM_LINUX
#if ENTRY_CONFIG_USE_WAYLAND
#include <wayland-egl.h>
#endif
#elif BX_PLATFORM_WINDOWS
#define SDL_MAIN_HANDLED
#endif

#include "stratengine/macro.h"

// cppcheck-suppress unknownMacro
STRATE_INNER_NAMESPACE(platform)

enum class StratEngineAppResult {
  kOk = 0,
  kSDLInitError = 1,
  kCreateWindowError = 2,
  kWindowMInfoError = 3,
  kWindowAlreadyCreated = 4,
  kUnsupportedPlaform = 5,
  kRendererBackendInitFailed = 6,
};

enum class PlatformType {
  kUnsupportedPlatform = 0,
  kWindows = 1,
  kLinux = 2,
  kMacOSX = 3,
  kAndroid = 4,
  kIOS = 5,
  kWebGL = 6
};

enum class RenderedBackend {
  kOpenGL,
  kOpenGLES,
  kVulkan,
  kMetal,
};

class StratEngineApp {
 public:
  StratEngineApp()
                  : window_(nullptr),
                    native_window_handle_(nullptr),
                    ndt_display_(nullptr),
                    ptype_(GetPlatform()),
                    initialization_state_(Init()),
                    is_rendered_run_(false),
                    is_window_created_(false) {}

  ~StratEngineApp() {
    ShutdownRenderedBackend();
    ShutdownApp();
  }

  StratEngineAppResult InitApp(RenderedBackend backend, const std::string& text,
                               int width = 1280, int height = 720,
                               bool fullscreen = false) {
    if (window_)
      return StratEngineAppResult::kWindowAlreadyCreated;

    auto create_win_res =
        CreateWindowImpl(backend, window_, text, width, height, fullscreen);

    if (create_win_res != StratEngineAppResult::kOk)
      return StratEngineAppResult::kCreateWindowError;

    auto wmi_res = GetWmi(wmi_);
    if (wmi_res != StratEngineAppResult::kOk)
      return StratEngineAppResult::kCreateWindowError;

    auto nhandle_res = GetNativeWindowHandle(native_window_handle_);
    if (wmi_res != StratEngineAppResult::kOk)
      return StratEngineAppResult::kCreateWindowError;

    is_window_created_ = true;
    return StratEngineAppResult::kOk;
  }

  const RenderedBackend& Backend() const {
    return backend_;
  }

  const PlatformType& Platform() const {
    return ptype_;
  }

  StratEngineAppResult InitRenderedBackend() {
#if defined(_WIN32) || defined(_WIN64)
    ndt_display_ = nullptr;
#elif defined(__APPLE__)
#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
    ndt_display_ = wmi.info.cocoa.window;
    ;

#elif TARGET_OS_MAC
    ndt_display_ = wmi.info.cocoa.window;

#endif
#elif defined(__ANDROID__)
    ndt_display_ = wmi.info.android.window;
#elif defined(__linux__)
    if (wmi_.subsystem == SDL_SYSWM_WAYLAND) {
      ndt_display_ = wmi_.info.wl.display;

      // wl_egl_window *win_impl = (wl_egl_window*)SDL_GetWindowData(window_,
      // "wl_egl_window"); 		if(!win_impl)
      // 		{
      // 			int width, height;
      // 			SDL_GetWindowSize(window_, &width, &height);
      // 			struct wl_surface* surface =
      // wmi_.info.wl.surface;
      // if(!surface)
      // return StratEngineAppResult::kRendererBackendInitFailed;
      // win_impl = wl_egl_window_create(surface, width, height);
      // 			SDL_SetWindowData(window_, "wl_egl_window",
      // win_impl);
      // 		}
      // 		ndt_display_ (void*)(uintptr_t)win_impl;

      std::cout << "Linux (Wayland) native display handle obtained."
                << std::endl;

    } else if (wmi_.subsystem == SDL_SYSWM_X11) {
      ndt_display_ = wmi_.info.x11.display;
      std::cout << "Linux (X11) native display handle obtained." << std::endl;
    } else {
      return StratEngineAppResult::kUnsupportedPlaform;
    }
#endif

    bgfx::Init init;
    init.vendorId = BGFX_PCI_ID_NONE;
    init.type = GetBgfxRendererType(backend_);
    init.platformData.nwh = native_window_handle_;
    init.platformData.ndt = ndt_display_;
    init.resolution.width = 1280;
    init.resolution.height = 720;
    init.resolution.reset = BGFX_RESET_VSYNC;

    if (!bgfx::init(init)) {
      std::cerr << "bgfx::init failed!" << std::endl;
      return StratEngineAppResult::kRendererBackendInitFailed;
    }

    is_rendered_run_ = true;

    return StratEngineAppResult::kOk;
  }

  void ShutdownRenderedBackend() {
    if(!is_rendered_run_)
      return;
    bgfx::shutdown();
    is_rendered_run_ = false;
  }

  const StratEngineAppResult& IsCanRun() const {
    return initialization_state_;
  }

  void ShutdownApp() {
    if (!is_window_created_)
      return;

    is_window_created_ = false;

    SDL_DestroyWindow(window_);
    SDL_Quit();

  }

 private:
  SDL_Window* window_;
  void* native_window_handle_;
  void* ndt_display_;
  SDL_SysWMinfo wmi_;
  PlatformType ptype_;
  StratEngineAppResult initialization_state_;
  RenderedBackend backend_;
  bool is_rendered_run_;
  bool is_window_created_; 

  bgfx::NativeWindowHandleType::Enum GetNativeWindowHandleType() {
    return bgfx::NativeWindowHandleType::Default;
  }

  bgfx::RendererType::Enum GetBgfxRendererType(const RenderedBackend& backend) {
    switch (backend) {
      case RenderedBackend::kOpenGL:
        return bgfx::RendererType::OpenGL;
        break;

      case RenderedBackend::kOpenGLES:
        return bgfx::RendererType::OpenGLES;
        break;

      case RenderedBackend::kVulkan:
        return bgfx::RendererType::Vulkan;
        break;

      case RenderedBackend::kMetal:
        return bgfx::RendererType::Metal;
        break;

      default:
        return bgfx::RendererType::OpenGL;
        break;
    }
  }

  SDL_WindowFlags GetSDLRenderedFlag(const RenderedBackend& backend) {
    switch (backend) {
      case RenderedBackend::kOpenGL:
        return SDL_WINDOW_OPENGL;
        break;

      case RenderedBackend::kOpenGLES:
        return SDL_WINDOW_OPENGL;
        break;

      case RenderedBackend::kVulkan:
        return SDL_WINDOW_VULKAN;
        break;

      case RenderedBackend::kMetal:
        return SDL_WINDOW_METAL;
        break;

      default:
        return SDL_WINDOW_OPENGL;
        break;
    }
  }

  PlatformType GetPlatform() {
#if defined(_WIN32) || defined(_WIN64)
    return PlatformType::kWindows;

#elif defined(__APPLE__)
#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
    return PlatformType::kIOS;
#elif TARGET_OS_MAC
    return PlatformType::kMacOSX;
#endif
#elif defined(__ANDROID__)
    return PlatformType::Android;
#elif defined(__linux__)
    return PlatformType::kLinux;
#endif
    return PlatformType::kUnsupportedPlatform;
  }

  StratEngineAppResult Init() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
      std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
      return StratEngineAppResult::kSDLInitError;
    }

    return StratEngineAppResult::kOk;
  }

  StratEngineAppResult CreateWindowImpl(const RenderedBackend& backend,
                                        SDL_Window* window,
                                        const std::string& text,
                                        int width = 1280, int height = 720,
                                        bool fullscreen = false) {
    window_ = SDL_CreateWindow(
        text.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width,
        height,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | GetSDLRenderedFlag(backend));

    if (!window_) {
      std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
      return StratEngineAppResult::kCreateWindowError;
    }

    return StratEngineAppResult::kOk;
  }

  StratEngineAppResult GetWmi(SDL_SysWMinfo& wmi) {
    SDL_VERSION(&wmi.version);
    if (!SDL_GetWindowWMInfo(window_, &wmi)) {
      std::cerr << "SDL_GetWindowWMInfo Error: " << SDL_GetError() << std::endl;
      return StratEngineAppResult::kWindowMInfoError;
    }

    return StratEngineAppResult::kOk;
  }

  StratEngineAppResult GetNativeWindowHandle(void* native_window_handle) {
#if defined(_WIN32) || defined(_WIN64)
    native_window_handle = (void*)wmi_.info.win.window;

#elif defined(__APPLE__)
#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
    native_window_handle = wmi_.info.uikit.window;
#elif TARGET_OS_MAC
    native_window_handle = (void*)wmi_.info.cocoa.window;
#endif
#elif defined(__ANDROID__)
    native_window_handle = (void*)wmi_.info.android.window;
#elif defined(__linux__)
    if (wmi_.subsystem == SDL_SYSWM_X11) {
      native_window_handle_ = (void*)(uintptr_t)wmi_.info.x11.window;
    } else if (wmi_.subsystem == SDL_SYSWM_WAYLAND) {
      native_window_handle_ = (void*)(uintptr_t)wmi_.info.wl.egl_window;
    } else {
      // std::cerr << "Unsupported Linux window manager subsystem!" <<
      // std::endl;
      return StratEngineAppResult::kUnsupportedPlaform;
    }
#else
    return StratEngineAppResult::kUnsupportedPlaform;
#endif

    return StratEngineAppResult::kOk;
  }
};

STRATE_INNER_END_NAMESPACE