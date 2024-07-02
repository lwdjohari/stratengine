#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <stdexcept>
#include "vk_mem_alloc.h"
#include "VkBootstrap.h"
#include "stratengine/macro.h"

STRATE_INNER_NAMESPACE(editor)
STRATE_MAKE_NAMESPACE(renderer)

class VulkanContext {
public:
    VulkanContext();
    ~VulkanContext();
    
    void Initialize();
    void Cleanup();

    VkInstance GetInstance() const;
    VkDevice GetDevice() const;
    VkPhysicalDevice GetPhysicalDevice() const;
    VkQueue GetQueue() const;
    uint32_t GetQueueFamily() const;
    VkCommandBuffer CreateCommandBuffer(VkCommandPool commandPool);

private:
    void CreateInstance();
    void SelectPhysicalDevice();
    void CreateLogicalDevice();
    void CreateCommandPool();

    VkInstance instance_;
    VkPhysicalDevice physicalDevice_;
    VkDevice device_;
    VkQueue queue_;
    uint32_t queueFamily_;
    VkCommandPool commandPool_;
};
STRATE_END_NAMESPACE
STRATE_INNER_END_NAMESPACE