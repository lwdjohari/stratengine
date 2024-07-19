#include "stratengine/editor/backend/vulkan_context.h"

#include <SDL_vulkan.h>
#include <fmt/core.h>

#include <iostream>
#include <stdexcept>

// cppcheck-suppress unknownMacro
STRATE_INNER_NAMESPACE(editor)
STRATE_MAKE_NAMESPACE(backend)

VulkanContext::VulkanContext()
                : surface_(VK_NULL_HANDLE),
                  graphics_queue_(VK_NULL_HANDLE),
                  graphics_queue_family_index_(UINT32_MAX) {}

VulkanContext::~VulkanContext() {}

void VulkanContext::Destroy() {
  physical_devices_.clear();
  debug_callback_ = nullptr;

  if (surface_ != VK_NULL_HANDLE) {
    vkDestroySurfaceKHR(instance_.instance, surface_, nullptr);
  }

  // if (instance_.instance != VK_NULL_HANDLE) {
  //   vkDestroyInstance(instance_.instance, nullptr);
  // }

  if (device_)
    vkb::destroy_device(device_);

  if (instance_)
    vkb::destroy_instance(instance_);
}

void VulkanContext::SetDebugCallback(VkDebugCallback callback) {
  debug_callback_ = callback;
}
void VulkanContext::RemoveDebugCallback() {
  debug_callback_ = nullptr;
}

VkBackendResult VulkanContext::CreateVkSurface(SDL_Window* window) {
  if (!instance_)
    return VkBackendResult::ErrorInitializationFailed;

  auto surface_result =
      SDL_Vulkan_CreateSurface(window, instance_.instance, &surface_);

  if (!surface_result) {
    return VkBackendResult::ErrorInitializationFailed;
  }

  vkb::PhysicalDeviceSelector selector(instance_);

  auto physical_devices_result =
      selector.set_minimum_version(API_MIN_VER_MAJOR, API_MIN_VER_MINOR)
          .set_surface(surface_)
          .select_devices();

  if (!physical_devices_result) {
    fmt::print("Vulkan Get GPUs: {} gpus\n", "Failed to get");
  }

  if (physical_devices_result.has_value()) {
    physical_devices_ = physical_devices_result.value();
  }

  return VkBackendResult::Success;

  // physical_device_result.if (!physical_device_result){
  //     return physical_device_result.} physical_device_ =
  //     physical_device_result.value();

  // // Select device features
  // VkPhysicalDeviceFeatures device_features = physical_device_.features;
  // // Enable all available features here if needed

  // // Create device
  // vkb::DeviceBuilder device_builder(physical_device_);
  // auto device_result = device_builder.add_pNext(&device_features).build();

  // if (!device_result) {
  //   return device_result.full_error();
  // }

  // device_ = device_result.value();

  // graphics_queue_ = device_.get_queue(vkb::QueueType::graphics).value();
  // graphics_queue_family_index_ =
  //     device_.get_queue_index(vkb::QueueType::graphics).value();
}

VkBackendResult VulkanContext::Initialize(SDL_Window* window) {
  if (!window)
    return VkBackendResult::ErrorInitializationFailed;

  // Create instance
  vkb::InstanceBuilder instance_builder;
  instance_builder =
      instance_builder.set_app_name("Strateditor")
          .set_engine_name("stratengine")
          .require_api_version(API_MIN_VER_MAJOR, API_MIN_VER_MINOR, 0)
          .request_validation_layers(true);

  instance_builder = debug_callback_ != nullptr
                         ? instance_builder.set_debug_callback(debug_callback_)
                         : instance_builder.use_default_debug_messenger();

  auto system_info_ret = vkb::SystemInfo::get_system_info();
  if (!system_info_ret) {
    return VkBackendResult::ErrorInitializationFailed;
  }

  auto system_info = system_info_ret.value();
  if (system_info.is_layer_available("VK_LAYER_LUNARG_api_dump")) {
    instance_builder =
        instance_builder.enable_layer("VK_LAYER_LUNARG_api_dump");
  }

  if (system_info.validation_layers_available) {
    instance_builder = instance_builder.enable_validation_layers();
  }

  auto instance_result = instance_builder.build();

  if (!instance_result) {
    return VkBackendResult::ErrorInitializationFailed;
  }

  instance_ = instance_result.value();

  is_vk_initialized_ = true;

  if (CreateVkSurface(window) != VkBackendResult::Success) {
    Destroy();
    is_vk_initialized_ = false;

    return VkBackendResult::ErrorInitializationFailed;
  }

  return VkBackendResult::Success;
}

std::vector<vkb::PhysicalDevice>& VulkanContext::GetPhysicalDevices() {
  return physical_devices_;
}

const std::vector<vkb::PhysicalDevice>& VulkanContext::GetPhysicalDevices()
    const {
  return physical_devices_;
}

VkBackendResult VulkanContext::SelectDevice(
    vkb::PhysicalDevice* physical_device) {
  if (!is_vk_initialized_)
    return VkBackendResult::ErrorInitializationFailed;

  if (!physical_device)
    return VkBackendResult::ErrorInvalidExternalHandle;


   

  // VkPhysicalDeviceFeatures device_features = physical_device->features;
  

  // device_features.geometryShader = VK_TRUE;
  // device_features.multiDrawIndirect = VK_TRUE;
  // device_features.multiViewport = VK_TRUE;
  // device_features.textureCompressionETC2 = VK_TRUE;

  vkb::DeviceBuilder device_builder(*(physical_device));
  vkb::Device vkb_device =
      device_builder.build().value();

  if (!vkb_device)
    return VkBackendResult::ErrorInvalidExternalHandle;

  device_ = vkb_device;
  physical_device_ = *physical_device;

  is_vk_device_selected_ = true;
  return VkBackendResult::Success;
}

vkb::Instance VulkanContext::GetInstance() const {
  return instance_;
}

const vkb::PhysicalDevice* VulkanContext::GetPhysicalDevice() const {
  return &physical_device_;
}

vkb::Device* VulkanContext::GetDevice() {
  return &device_;
}

const vkb::Device* VulkanContext::GetDevice() const {
  return &device_;
}

VkSurfaceKHR VulkanContext::GetSurface() const {
  return surface_;
}

VkQueue VulkanContext::GetGraphicsQueue() const {
  return graphics_queue_;
}

uint32_t VulkanContext::GetGraphicsQueueFamilyIndex() const {
  return graphics_queue_family_index_;
}

STRATE_END_NAMESPACE
STRATE_INNER_END_NAMESPACE
