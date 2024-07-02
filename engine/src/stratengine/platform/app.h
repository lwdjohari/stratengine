#pragma once

#include <GLFW/glfw3.h>

#include <iostream>

#if STRATE_PLATFORM_LINUX == 1
#include <wayland-egl.h>
#endif

#include "stratengine/macro.h"

STRATE_INNER_NAMESPACE(platform)

enum class StratEngineAppResult {
  kOk = 0,
  kGLFWInitError = 1,
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
  kOffScreen = 8
};

struct BackendInfo {
  RenderedBackend backend;
  int major_version;
  int minor_version;
};

STRATE_ENUM_CLASS_ENABLE_BITMASK_OPERATORS(RenderedBackend)

STRATE_ENUM_CLASS_DISPLAY_TRAIT(RenderedBackend)

STRATE_ENUM_CLASS_TO_STRING_FORMATTER(RenderedBackend,
                                      case RenderedBackend::kOffScreen
                                      : return "Offscreen";
                                      case RenderedBackend::kNone
                                      : return "None";
                                      case RenderedBackend::kOpenGL
                                      : return "OpenGL";
                                      case RenderedBackend::kOpenGLES
                                      : return "OpenGLES";
                                      case RenderedBackend::kVulkan
                                      : return "Vulkan";);

static std::vector<BackendInfo> QuerySupportedBackends() {
  std::vector<BackendInfo> backends;

  bool terminateGlfw = false;
  if (!glfwInit()) {
    if (!glfwInit()) {
      std::cerr << "Failed to initialize GLFW" << std::endl;
      return backends;
    }
    terminateGlfw = true;
  }

  // Query Vulkan backends
  if (glfwVulkanSupported()) {
    backends.push_back({RenderedBackend::kVulkan, 1,
                        0});  // Vulkan version is generally handled differently
  }



  // Query OpenGL versions

  
  for (int major = 1; major <= 4; ++major) {
    for (int minor = 0; minor <= 10; ++minor) {
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
      glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
      GLFWwindow* window = glfwCreateWindow(1, 1, "", nullptr, nullptr);
      if (window) {
        backends.push_back({RenderedBackend::kOpenGL, major, minor});
        glfwDestroyWindow(window);
      }
    }
  }

  // Query OpenGL ES versions
  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
  for (int major = 1; major <= 3; ++major) {
    for (int minor = 0; minor <= 6; ++minor) {
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
      glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
      GLFWwindow* window = glfwCreateWindow(1, 1, "", nullptr, nullptr);
      if (window) {
        backends.push_back({RenderedBackend::kOpenGLES, major, minor});
        glfwDestroyWindow(window);
      }
    }
  }

  if (terminateGlfw) {
    glfwTerminate();
  }

  return backends;
}

static LinuxDisplayType GetLinuxDisplayType() {
#if STRATE_PLATFORM_LINUX == 1
  LinuxDisplayType type = LinuxDisplayType::kNone;

  // Check for Wayland environment variable
  if (std::getenv("WAYLAND_DISPLAY")) {
    type = static_cast<LinuxDisplayType>(
        static_cast<int>(type) | static_cast<int>(LinuxDisplayType::kWayland));
  }

  // Check for X11 environment variable
  if (std::getenv("DISPLAY")) {
    type = static_cast<LinuxDisplayType>(
        static_cast<int>(type) | static_cast<int>(LinuxDisplayType::kX11));
  }

  return type;
#else
  return LinuxDisplayType::kNone;
#endif
}

static PlatformType GetPlatform() {
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

// static RenderedBackend DetermineRenderedBackend(RenderedBackend target,
//                                                 RenderedBackend fallback) {
//   auto backends = GetSupportedRenderedBackend();

//   if ((backends & target) != RenderedBackend::kNone) {
//     return target;
//   }

//   if ((backends & fallback) != RenderedBackend::kNone) {
//     return fallback;
//   }

//   return RenderedBackend::kNone;
// }

class StratEngineApp {
 public:
  StratEngineApp()
                  : window_(nullptr),
                    ptype_(GetPlatform()),
                    initialization_state_(Init()),
                    is_rendered_run_(false),
                    is_window_created_(false) {}

  ~StratEngineApp() {
   
    ShutdownApp();
  }

  StratEngineAppResult InitApp(RenderedBackend backend, const std::string& text,
                               int width = 1280, int height = 720,
                               bool fullscreen = false) {
    if (window_)
      return StratEngineAppResult::kWindowAlreadyCreated;

    auto create_win_res =
        CreateWindowImpl(backend, text, width, height, fullscreen);

    if (create_win_res != StratEngineAppResult::kOk)
      return StratEngineAppResult::kCreateWindowError;

    is_window_created_ = true;
    is_rendered_run_ = true;
    return StratEngineAppResult::kOk;
  }

  const RenderedBackend& Backend() const {
    return backend_;
  }

  const PlatformType& Platform() const {
    return ptype_;
  }

  GLFWwindow* GetWindow(){
    return window_;
  }

  const GLFWwindow* GetWindow() const {
    return window_;
  }

  const StratEngineAppResult& IsCanRun() const {
    return initialization_state_;
  }

  void ShutdownApp() {
    if (!is_window_created_) {
      return;
    }

    is_window_created_ = false;

    glfwDestroyWindow(window_);
    glfwTerminate();

    is_rendered_run_ = false;

  }

 private:
  GLFWwindow* window_;
  PlatformType ptype_;
  StratEngineAppResult initialization_state_;
  RenderedBackend backend_;
  bool is_rendered_run_;
  bool is_window_created_;

  StratEngineAppResult Init() {
    if (!glfwInit()) {
      std::cerr << "GLFW initialization failed!" << std::endl;
      return StratEngineAppResult::kGLFWInitError;
    }
    return StratEngineAppResult::kOk;
  }

  StratEngineAppResult CreateWindowImpl(const RenderedBackend& backend,
                                        const std::string& text,
                                        int width = 1280, int height = 720,
                                        bool fullscreen = false) {
    if (backend == RenderedBackend::kOpenGL ||
        backend == RenderedBackend::kOpenGLES) {
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
      glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
      if (backend == RenderedBackend::kOpenGLES) {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
      }
    } else if (backend == RenderedBackend::kVulkan) {
      glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    }

    glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
    window_ = glfwCreateWindow(width, height, text.c_str(), nullptr, nullptr);
    if (!window_) {
      std::cerr << "GLFW window creation failed!" << std::endl;
      return StratEngineAppResult::kCreateWindowError;
    }

    if (backend != RenderedBackend::kVulkan) {
      glfwMakeContextCurrent(window_);
    }

    return StratEngineAppResult::kOk;
  }
};

STRATE_INNER_END_NAMESPACE