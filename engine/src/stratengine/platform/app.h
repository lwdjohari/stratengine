#pragma once
#include <SDL.h>
#include <SDL_syswm.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

#include <iostream>

#if STRATE_PLATFORM_LINUX == 1
#include <wayland-egl.h>
#endif

#include "stratengine/macro.h"

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

enum class LinuxDisplayType { kNone = 0, kX11 = 1, kWayland = 2 };

STRATE_ENUM_CLASS_ENABLE_BITMASK_OPERATORS(LinuxDisplayType)

STRATE_ENUM_CLASS_DISPLAY_TRAIT(LinuxDisplayType)

STRATE_ENUM_CLASS_TO_STRING_FORMATTER(LinuxDisplayType,
                                      case LinuxDisplayType::kNone
                                      // cppcheck-suppress syntaxError
                                      : return "None";
                                      case LinuxDisplayType::kWayland
                                      : return "Wayland";
                                      case LinuxDisplayType::kX11
                                      : return "X11";);

enum class RenderedBackend {
  kNone = 0,
  kOpenGL = 1,
  kOpenGLES = 2,
  kVulkan = 4,
  kMetal = 8,
  kDirect3D11 = 16,
  kDirect3D12 = 32,
  kOffScreen = 64,
  kDummy = 128,
  kNoop = 256,
};

STRATE_ENUM_CLASS_ENABLE_BITMASK_OPERATORS(RenderedBackend)

STRATE_ENUM_CLASS_DISPLAY_TRAIT(RenderedBackend)

STRATE_ENUM_CLASS_TO_STRING_FORMATTER(RenderedBackend,
                                      case RenderedBackend::kDirect3D11
                                      : return "Direct3D11";
                                      case RenderedBackend::kDirect3D12
                                      : return "Direct3D12";
                                      case RenderedBackend::kDummy
                                      : return "Dummy";
                                      case RenderedBackend::kMetal
                                      : return "Metal";
                                      case RenderedBackend::kOffScreen
                                      : return "Offscreen";
                                      case RenderedBackend::kNone
                                      : return "None";
                                      case RenderedBackend::kNoop
                                      : return "Noop";
                                      case RenderedBackend::kOpenGL
                                      : return "OpenGL";
                                      case RenderedBackend::kOpenGLES
                                      : return "OpenGLES";
                                      case RenderedBackend::kVulkan
                                      : return "Vulkan";);

static RenderedBackend FromBgfxRenderType(bgfx::RendererType::Enum renderer) {
  switch (renderer) {
    case bgfx::RendererType::OpenGL:
      return RenderedBackend::kOpenGL;
      break;
    case bgfx::RendererType::OpenGLES:
      return RenderedBackend::kOpenGLES;
      break;
    case bgfx::RendererType::Vulkan:
      return RenderedBackend::kVulkan;
      break;
    case bgfx::RendererType::Metal:
      return RenderedBackend::kMetal;
    case bgfx::RendererType::Direct3D11:
      return RenderedBackend::kDirect3D11;
    case bgfx::RendererType::Direct3D12:
      return RenderedBackend::kDirect3D12;
    case bgfx::RendererType::Noop:
      return RenderedBackend::kNoop;
    default:
      return RenderedBackend::kNone;
  }

  return RenderedBackend::kNone;
}

static RenderedBackend GetSupportedRenderedBackend() {
  bgfx::RendererType::Enum supportedRenderers[bgfx::RendererType::Count];
  uint8_t numSupported = bgfx::getSupportedRenderers(bgfx::RendererType::Count,
                                                     supportedRenderers);

  RenderedBackend stratengine_backends = RenderedBackend::kDummy |
                                         RenderedBackend::kNoop |
                                         RenderedBackend::kOffScreen;

  for (uint8_t i = 0; i < numSupported; ++i) {
    // const char* name = bgfx::getRendererName(supportedRenderers[i]);
    // std::cout << " - " << name << std::endl;

    RenderedBackend backend = FromBgfxRenderType(supportedRenderers[i]);
    if (backend != RenderedBackend::kNone ||
        backend != RenderedBackend::kNoop) {
      stratengine_backends = stratengine_backends | backend;
    }
  }

  return stratengine_backends;
}

static LinuxDisplayType GetLinuxDisplayType() {
  int numVideoDrivers = SDL_GetNumVideoDrivers();

  LinuxDisplayType type = LinuxDisplayType::kNone;

  for (int i = 0; i < numVideoDrivers; ++i) {
    const char* driverName = SDL_GetVideoDriver(i);
    if (strcmp(driverName, "wayland") == 0) {
      type = type | LinuxDisplayType::kWayland;
    } else if (strcmp(driverName, "x11") == 0) {
      type = type | LinuxDisplayType::kX11;
    }
  }

  return type;
}

static LinuxDisplayType DetermineLinuxDisplayType(LinuxDisplayType target,
                                                  LinuxDisplayType fallback) {
  auto displays = GetLinuxDisplayType();

  if ((displays & target) != LinuxDisplayType::kNone) {
    return target;
  }

  if ((displays & fallback) != LinuxDisplayType::kNone) {
    return fallback;
  }

  return LinuxDisplayType::kNone;
}

static RenderedBackend DetermineRenderedBackend(RenderedBackend target,
                                                RenderedBackend fallback) {
  auto backends = GetSupportedRenderedBackend();

  if ((backends & target) != RenderedBackend::kNone) {
    return target;
  }

  if ((backends & fallback) != RenderedBackend::kNone) {
    return fallback;
  }

  return RenderedBackend::kNone;
}

class StratEngineApp {
 public:
  StratEngineApp()
                  : window_(nullptr),
                    native_window_handle_(nullptr),
                    ndt_display_(nullptr),
                    ptype_(GetPlatform()),
                    initialization_state_(Init()),
                    linux_display_type_(),
                    is_rendered_run_(false),
                    is_window_created_(false) {}

  ~StratEngineApp() {
    ShutdownRenderedBackend();
    ShutdownApp();
  }

  StratEngineAppResult InitApp(RenderedBackend backend, const std::string& text,
                               bool use_wayland_if_avail_in_linux) {
    return InitApp(backend, text, 1280, 720, false,
                   use_wayland_if_avail_in_linux);
  }

  StratEngineAppResult InitApp(RenderedBackend backend, const std::string& text,
                               int width = 1280, int height = 720,
                               bool fullscreen = false,
                               bool use_wayland_if_avail_in_linux = true) {
    if (window_)
      return StratEngineAppResult::kWindowAlreadyCreated;

#if STRATE_PLATFORM_LINUX == 1
    if (use_wayland_if_avail_in_linux) {
      linux_display_type_ = LinuxDisplayType::kWayland;
    } else {
      linux_display_type_ = LinuxDisplayType::kX11;
    }
#else
    linux_display_type_ = LinuxDisplayType::kNone;
#endif

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

  const LinuxDisplayType& LinuxDisplay() const {
    return linux_display_type_;
  }

  const RenderedBackend& Backend() const {
    return backend_;
  }

  const PlatformType& Platform() const {
    return ptype_;
  }

  StratEngineAppResult InitRenderedBackend() {
#if STRATE_PLATFORM_WINDOWS == 1
    ndt_display_ = nullptr;
#elif STRATE_PLATFORM_IOS == 1
#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
    ndt_display_ = wmi.info.cocoa.window;
#elif STRATE_PLATFORM_OSX
    ndt_display_ = wmi.info.cocoa.window;
#endif
#elif STRATE_PLATFORM_ANDROID == 1
    ndt_display_ = wmi.info.android.window;
#elif STRATE_PLATFORM_LINUX == 1
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
    if (!is_rendered_run_)
      return;
    bgfx::shutdown();
    is_rendered_run_ = false;
  }

  const StratEngineAppResult& IsCanRun() const {
    return initialization_state_;
  }

  void ShutdownApp() {
    if (!is_window_created_) {
      SDL_Quit();
      return;
    }

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
  LinuxDisplayType linux_display_type_;
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
#if STRATE_PLATFORM_WINDOWS == 1
    return PlatformType::kWindows;
#elif STRATE_PLATFORM_IOS == 1
#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
    return PlatformType::kIOS;
#endif
#elif STRATE_PLATFORM_IOS == 1
    return PlatformType::kMacOSX;
#elif STRATE_PLATFORM_ANDROID == 1
    return PlatformType::kAndroid;
#elif STRATE_PLATFORM_LINUX == 1
    return PlatformType::kLinux;
#else
    return PlatformType::kUnsupportedPlatform;
#endif
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
#if STRATE_PLATFORM_WINDOWS == 1
    native_window_handle = (void*)wmi_.info.win.window;
#elif STRATE_PLATFORM_IOS == 1
#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
    native_window_handle = wmi_.info.uikit.window;
#endif
#elif STRATE_PLATFORM_OSX == 1
    native_window_handle = (void*)wmi_.info.cocoa.window;
#elif STRATE_PLATFORM_ANDROID == 1
    native_window_handle = (void*)wmi_.info.android.window;
#elif STRATE_PLATFORM_LINUX == 1
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