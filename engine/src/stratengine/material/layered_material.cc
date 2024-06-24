#include "stratengine/material/layered_material.h"

// cppcheck-suppress unknownMacro
STRATE_INNER_NAMESPACE(material)

void LayeredMaterial::AddLayer(Material* material, float blendFactor) {
  layers_.push_back({material, blendFactor});
}

void LayeredMaterial::Apply(PassType pass_type) {
  for (const auto& layer : layers_) {
    layer.material->Apply(pass_type);
    // Blend layer with the blendFactor
    bgfx::setUniform(
        bgfx::createUniform("u_blendFactor", bgfx::UniformType::Vec4),
        &layer.blendFactor);
  }
}

bgfx::ProgramHandle LayeredMaterial::GetProgram() const {
  if (!layers_.empty()) {
    return layers_.front()
        .material->GetProgram();  // Assuming all layers use the same program
  }
  return BGFX_INVALID_HANDLE;
}

STRATE_INNER_END_NAMESPACE