#pragma once
#include <vector>

#include "stratengine/material/material.h"

STRATE_INNER_NAMESPACE(material)

struct MaterialLayer {
  Material* material;
  float blendFactor;
};

class LayeredMaterial {
 public:
  void AddLayer(Material* material, float blendFactor);
  void Apply(PassType pass_type);
  bgfx::ProgramHandle GetProgram() const;

 private:
  std::vector<MaterialLayer> layers_;
};

STRATE_INNER_END_NAMESPACE