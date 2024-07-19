#pragma once

#include <vulkan/vulkan.h>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_vulkan.h>
#include <SDL.h>

#include "stratengine/macro.h"

STRATE_INNER_NAMESPACE(editor)
STRATE_MAKE_NAMESPACE(backend)

class VulkanContext;

class ImGuiContext{
public:
    ImGuiContext(SDL_Window* window, VulkanContext* vulkanContext);
    ~ImGuiContext();

    void Initialize();
    void Cleanup();

    void BeginFrame();
    void EndFrame(VkCommandBuffer commandBuffer);

private:
    void CreateDescriptorPool();

    SDL_Window* window_;
    VulkanContext* vulkanContext_;
    VkDescriptorPool descriptor_pool_;
};

STRATE_END_NAMESPACE
STRATE_INNER_END_NAMESPACE