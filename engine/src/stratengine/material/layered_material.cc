#include "stratengine/material/layered_material.h"

// cppcheck-suppress unknownMacro
STRATE_INNER_NAMESPACE(material)

void LayeredMaterial::AddLayer(Material* material, float blendFactor) {
  layers_.push_back({material, blendFactor});
}

void LayeredMaterial::Apply(PassType pass_type) {
  for (const auto& layer : layers_) {
    layer.material->Apply(pass_type);
    
  }
}

void* LayeredMaterial::GetProgram() const {
  if (!layers_.empty()) {
    
  }
  return nullptr;
}

STRATE_INNER_END_NAMESPACE