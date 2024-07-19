#pragma once

#include <SDL.h>
#include <vulkan/vulkan.h>

#include <optional>
#include <vector>

#include "VkBootstrap.h"
#include "stratengine/macro.h"
STRATE_INNER_NAMESPACE(editor)
STRATE_MAKE_NAMESPACE(backend)

enum class VkBackendResult {
  Success = VK_SUCCESS,
  NotReady = VK_NOT_READY,
  Timeout = VK_TIMEOUT,
  EventSet = VK_EVENT_SET,
  EventReset = VK_EVENT_RESET,
  Incomplete = VK_INCOMPLETE,
  ErrorOutOfHostMemory = VK_ERROR_OUT_OF_HOST_MEMORY,
  ErrorOutOfDeviceMemory = VK_ERROR_OUT_OF_DEVICE_MEMORY,
  ErrorInitializationFailed = VK_ERROR_INITIALIZATION_FAILED,
  ErrorDeviceLost = VK_ERROR_DEVICE_LOST,
  ErrorMemoryMapFailed = VK_ERROR_MEMORY_MAP_FAILED,
  ErrorLayerNotPresent = VK_ERROR_LAYER_NOT_PRESENT,
  ErrorExtensionNotPresent = VK_ERROR_EXTENSION_NOT_PRESENT,
  ErrorFeatureNotPresent = VK_ERROR_FEATURE_NOT_PRESENT,
  ErrorIncompatibleDriver = VK_ERROR_INCOMPATIBLE_DRIVER,
  ErrorTooManyObjects = VK_ERROR_TOO_MANY_OBJECTS,
  ErrorFormatNotSupported = VK_ERROR_FORMAT_NOT_SUPPORTED,
  ErrorFragmentedPool = VK_ERROR_FRAGMENTED_POOL,
  ErrorUnknown = VK_ERROR_UNKNOWN,
  ErrorOutOfPoolMemory = VK_ERROR_OUT_OF_POOL_MEMORY,
  ErrorInvalidExternalHandle = VK_ERROR_INVALID_EXTERNAL_HANDLE,
  ErrorFragmentation = VK_ERROR_FRAGMENTATION,
  ErrorInvalidOpaqueCaptureAddress = VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS,
  PipelineCompileRequired = VK_PIPELINE_COMPILE_REQUIRED,
  ErrorSurfaceLostKHR = VK_ERROR_SURFACE_LOST_KHR,
  ErrorNativeWindowInUseKHR = VK_ERROR_NATIVE_WINDOW_IN_USE_KHR,
  SuboptimalKHR = VK_SUBOPTIMAL_KHR,
  ErrorOutOfDateKHR = VK_ERROR_OUT_OF_DATE_KHR,
  ErrorIncompatibleDisplayKHR = VK_ERROR_INCOMPATIBLE_DISPLAY_KHR,
  ErrorValidationFailedEXT = VK_ERROR_VALIDATION_FAILED_EXT,
  ErrorInvalidShaderNV = VK_ERROR_INVALID_SHADER_NV,
  ErrorImageUsageNotSupportedKHR = VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR,
  ErrorVideoPictureLayoutNotSupportedKHR =
      VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR,
  ErrorVideoProfileOperationNotSupportedKHR =
      VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR,
  ErrorVideoProfileFormatNotSupportedKHR =
      VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR,
  ErrorVideoProfileCodecNotSupportedKHR =
      VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR,
  ErrorVideoStdVersionNotSupportedKHR =
      VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR,
  ErrorInvalidDrmFormatModifierPlaneLayoutEXT =
      VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT,
  ErrorNotPermittedKHR = VK_ERROR_NOT_PERMITTED_KHR,
  ErrorFullScreenExclusiveModeLostEXT =
      VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT,
  ThreadIdleKHR = VK_THREAD_IDLE_KHR,
  ThreadDoneKHR = VK_THREAD_DONE_KHR,
  OperationDeferredKHR = VK_OPERATION_DEFERRED_KHR,
  OperationNotDeferredKHR = VK_OPERATION_NOT_DEFERRED_KHR,
  ErrorInvalidVideoStdParametersKHR = VK_ERROR_INVALID_VIDEO_STD_PARAMETERS_KHR,
  ErrorCompressionExhaustedEXT = VK_ERROR_COMPRESSION_EXHAUSTED_EXT,
  IncompatibleShaderBinaryEXT = VK_INCOMPATIBLE_SHADER_BINARY_EXT,
  ErrorOutOfPoolMemoryKHR = VK_ERROR_OUT_OF_POOL_MEMORY,
  ErrorInvalidExternalHandleKHR = VK_ERROR_INVALID_EXTERNAL_HANDLE,
  ErrorFragmentationEXT = VK_ERROR_FRAGMENTATION_EXT,
  ErrorNotPermittedEXT = VK_ERROR_NOT_PERMITTED_EXT,
  ErrorInvalidDeviceAddressEXT = VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS,
  ErrorInvalidOpaqueCaptureAddressKHR = VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS,
  PipelineCompileRequiredEXT = VK_PIPELINE_COMPILE_REQUIRED_EXT,
  ErrorPipelineCompileRequiredEXT = VK_PIPELINE_COMPILE_REQUIRED_EXT,
  ErrorIncompatibleShaderBinaryEXT = VK_INCOMPATIBLE_SHADER_BINARY_EXT,
  ResultMaxEnum = VK_RESULT_MAX_ENUM
};

static bool IsVkSuccess(const VkBackendResult& result) {
  if (result == VkBackendResult::Success)
    return true;

  return false;
}


class VulkanContext {
 public:
  VulkanContext();
  ~VulkanContext();

  constexpr const static int API_MIN_VER_MAJOR = 1;
  constexpr const static int API_MIN_VER_MINOR = 1;

  typedef VkBool32(VKAPI_PTR* VkDebugCallback)(
      VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
      VkDebugUtilsMessageTypeFlagsEXT messageType,
      const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
      void* pUserData);

  VkBackendResult Initialize(SDL_Window* window);
  VkBackendResult SelectDevice(vkb::PhysicalDevice* physical_device);
  vkb::Instance GetInstance() const;
  const vkb::PhysicalDevice* GetPhysicalDevice() const;
  const vkb::Device* GetDevice() const;
  vkb::Device* GetDevice();

  VkSurfaceKHR GetSurface() const;
  VkQueue GetGraphicsQueue() const;
  uint32_t GetGraphicsQueueFamilyIndex() const;
  std::vector<vkb::PhysicalDevice>& GetPhysicalDevices();
  const std::vector<vkb::PhysicalDevice>& GetPhysicalDevices() const;
  void SetDebugCallback(VkDebugCallback callback);
  void RemoveDebugCallback();
  void Destroy();

 private:
  vkb::Instance instance_;
  std::vector<vkb::PhysicalDevice> physical_devices_;
  vkb::PhysicalDevice physical_device_;
  vkb::Device device_;
  VkSurfaceKHR surface_;
  VkQueue graphics_queue_;
  uint32_t graphics_queue_family_index_;
  VkDebugCallback debug_callback_;
  bool is_vk_initialized_;
  bool is_vk_device_selected_;

  VkBackendResult CreateVkSurface(SDL_Window* window);
};

STRATE_END_NAMESPACE
STRATE_INNER_END_NAMESPACE