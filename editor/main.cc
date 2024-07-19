#include <fmt/core.h>
#include <stratengine/editor/backend/vulkan_context.h>
#include <stratengine/editor/window/main_window.h>

#include <iostream>

int main() {
  using namespace stratengine::editor;

  backend::VulkanContext vc;

  vc.SetDebugCallback(
      [](VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
         VkDebugUtilsMessageTypeFlagsEXT messageType,
         const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
         void* pUserData) -> VkBool32 {
        auto severity = vkb::to_string_message_severity(messageSeverity);
        auto type = vkb::to_string_message_type(messageType);
        printf("[%s: %s] %s\n", severity, type, pCallbackData->pMessage);
        return VK_FALSE;
      });

  window::MainWindow win("Stratengine Editor", 1280, 720);
  win.Initialize();

  auto init_res = vc.Initialize(win.GetSDLWindow());
  if (!backend::IsVkSuccess(init_res)) {
    fmt::print("[Vulkan: {}]\n", "Cant initialized Vulkan");
    return 0;
  }

  for (auto& d : vc.GetPhysicalDevices()) {
    fmt::print("[GPU]  {}\n", d.name);
  }

  backend::VkBackendResult select_device_res;
  if (vc.GetPhysicalDevices().size() > 0)
    select_device_res = vc.SelectDevice(&vc.GetPhysicalDevices()[0]);

  if (!backend::IsVkSuccess(select_device_res)) {
    fmt::print("Failed to select GPU \n");
    win.Destroy();
    vc.Destroy();
    return 0;
  }

  auto dev = vc.GetDevice();
   auto pdev = dev->physical_device;
  fmt::print("[Selected GPU]  {}\n", pdev.name);

  auto f = pdev.features;

  fmt::print("  MultiViewport:            {}\n",
             f.multiViewport ? "Supported" : "Not Supported");
  fmt::print("  Multidraw Indirect:       {}\n",
             f.multiDrawIndirect ? "Supported" : "Not Supported");
  fmt::print("  Geometry Shader:          {}\n",
             f.geometryShader ? "Supported" : "Not Supported");
  fmt::print("  Sparse Binding:           {}\n",
             f.sparseBinding ? "Supported" : "Not Supported");
  fmt::print("  Etc2 Compression:         {}\n",
             f.textureCompressionETC2 ? "Supported" : "Not Supported");
  
//   fmt::print("  Tessellation Shader:      {}\n",
//              f.tessellationShader ? "Supported" : "Not Supported");
//   fmt::print("  Wide Lines:               {}\n",
//              f.wideLines ? "Supported" : "Not Supported");
//   fmt::print("  Large Points:             {}\n",
//              f.largePoints ? "Supported" : "Not Supported");
//   fmt::print("  Depth Clamp:              {}\n",
//              f.depthClamp ? "Supported" : "Not Supported");
//   fmt::print("  Depth Bias Clamp:         {}\n",
//              f.depthBiasClamp ? "Supported" : "Not Supported");
//   fmt::print("  Fill Mode Non-Solid:      {}\n",
//              f.fillModeNonSolid ? "Supported" : "Not Supported");
//   fmt::print("  Independent Blend:        {}\n",
//              f.independentBlend ? "Supported" : "Not Supported");
//   fmt::print("  Sample Rate Shading:      {}\n",
//              f.sampleRateShading ? "Supported" : "Not Supported");
//   fmt::print("  Dual Src Blend:           {}\n",
//              f.dualSrcBlend ? "Supported" : "Not Supported");
//   fmt::print("  Logic Op:                 {}\n",
//              f.logicOp ? "Supported" : "Not Supported");
//   fmt::print("  Multi Draw Indirect:      {}\n",
//              f.multiDrawIndirect ? "Supported" : "Not Supported");
//   fmt::print("  Draw Indirect First Instance: {}\n",
//              f.drawIndirectFirstInstance ? "Supported" : "Not Supported");
//   fmt::print("  Depth Bounds:             {}\n",
//              f.depthBounds ? "Supported" : "Not Supported");
//   fmt::print("  Multi Viewport:           {}\n",
//              f.multiViewport ? "Supported" : "Not Supported");
//   fmt::print("  Sampler Anisotropy:       {}\n",
//              f.samplerAnisotropy ? "Supported" : "Not Supported");
//   fmt::print("  Texture Compression BC:   {}\n",
//              f.textureCompressionBC ? "Supported" : "Not Supported");
//   fmt::print("  Occlusion Query Precise:  {}\n",
//              f.occlusionQueryPrecise ? "Supported" : "Not Supported");
//   fmt::print("  Pipeline Statistics Query:{}\n",
//              f.pipelineStatisticsQuery ? "Supported" : "Not Supported");
//   fmt::print("  Vertex Pipeline Stores and Atomics: {}\n",
//              f.vertexPipelineStoresAndAtomics ? "Supported" : "Not Supported");
//   fmt::print("  Fragment Stores and Atomics: {}\n",
//              f.fragmentStoresAndAtomics ? "Supported" : "Not Supported");
//   fmt::print(
//       "  Shader Tessellation and Geometry Point Size: {}\n",
//       f.shaderTessellationAndGeometryPointSize ? "Supported" : "Not Supported");
//   fmt::print("  Shader Image Gather Extended: {}\n",
//              f.shaderImageGatherExtended ? "Supported" : "Not Supported");
//   fmt::print(
//       "  Shader Storage Image Extended Formats: {}\n",
//       f.shaderStorageImageExtendedFormats ? "Supported" : "Not Supported");
//   fmt::print("  Shader Storage Image Multisample: {}\n",
//              f.shaderStorageImageMultisample ? "Supported" : "Not Supported");
//   fmt::print(
//       "  Shader Storage Image Read Without Format: {}\n",
//       f.shaderStorageImageReadWithoutFormat ? "Supported" : "Not Supported");
//   fmt::print(
//       "  Shader Storage Image Write Without Format: {}\n",
//       f.shaderStorageImageWriteWithoutFormat ? "Supported" : "Not Supported");
//   fmt::print("  Shader Uniform Buffer Array Dynamic Indexing: {}\n",
//              f.shaderUniformBufferArrayDynamicIndexing ? "Supported"
//                                                        : "Not Supported");
//   fmt::print(
//       "  Shader Sampled Image Array Dynamic Indexing: {}\n",
//       f.shaderSampledImageArrayDynamicIndexing ? "Supported" : "Not Supported");
//   fmt::print("  Shader Storage Buffer Array Dynamic Indexing: {}\n",
//              f.shaderStorageBufferArrayDynamicIndexing ? "Supported"
//                                                        : "Not Supported");
//   fmt::print(
//       "  Shader Storage Image Array Dynamic Indexing: {}\n",
//       f.shaderStorageImageArrayDynamicIndexing ? "Supported" : "Not Supported");
//   fmt::print("  Shader Clip Distance: {}\n",
//              f.shaderClipDistance ? "Supported" : "Not Supported");
//   fmt::print("  Shader Cull Distance: {}\n",
//              f.shaderCullDistance ? "Supported" : "Not Supported");
//   fmt::print("  Shader Float64: {}\n",
//              f.shaderFloat64 ? "Supported" : "Not Supported");
//   fmt::print("  Shader Int64: {}\n",
//              f.shaderInt64 ? "Supported" : "Not Supported");
//   fmt::print("  Shader Int16: {}\n",
//              f.shaderInt16 ? "Supported" : "Not Supported");
//   fmt::print("  Shader Resource Residency: {}\n",
//              f.shaderResourceResidency ? "Supported" : "Not Supported");
//   fmt::print("  Shader Resource Min LOD: {}\n",
//              f.shaderResourceMinLod ? "Supported" : "Not Supported");
//   fmt::print("  Sparse Binding: {}\n",
//              f.sparseBinding ? "Supported" : "Not Supported");
//   fmt::print("  Sparse Residency Buffer: {}\n",
//              f.sparseResidencyBuffer ? "Supported" : "Not Supported");
//   fmt::print("  Sparse Residency Image 2D: {}\n",
//              f.sparseResidencyImage2D ? "Supported" : "Not Supported");
//   fmt::print("  Sparse Residency Image 3D: {}\n",
//              f.sparseResidencyImage3D ? "Supported" : "Not Supported");
//   fmt::print("  Sparse Residency 2 Samples: {}\n",
//              f.sparseResidency2Samples ? "Supported" : "Not Supported");
//   fmt::print("  Sparse Residency 4 Samples: {}\n",
//              f.sparseResidency4Samples ? "Supported" : "Not Supported");
//   fmt::print("  Sparse Residency 8 Samples: {}\n",
//              f.sparseResidency8Samples ? "Supported" : "Not Supported");
//   fmt::print("  Sparse Residency Aliased: {}\n",
//              f.sparseResidencyAliased ? "Supported" : "Not Supported");
//   fmt::print("  Variable Multisample Rate: {}\n",
//              f.variableMultisampleRate ? "Supported" : "Not Supported");
//   fmt::print("  Inherited Queries: {}\n",
//              f.inheritedQueries ? "Supported" : "Not Supported");

  bool running = true;
  SDL_Event event;

  while (running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        running = false;
      }
      // ImGui_ImplSDL2_ProcessEvent(&event);
    }

    // imguiContext.BeginFrame();

    // // Example ImGui dock space
    // ImGui::Begin("DockSpace Demo");
    // ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
    // ImGui::End();

    // // Example ImGui window
    // ImGui::Begin("Hello, world!");
    // ImGui::Text("This is some useful text.");
    // ImGui::End();

    // VkCommandBuffer command_buffer = vulkanContext.BeginCommandBuffer();
    // imguiContext.EndFrame(command_buffer);
    // vulkanContext.EndCommandBuffer(command_buffer);

    // Present the frame (this would need to be implemented)
  }

  win.Destroy();
  vc.Destroy();

  return 0;
}
