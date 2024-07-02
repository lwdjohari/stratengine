#pragma once

#include "stratengine/macro.h"
#include "stratengine/material/shader_effect.h"

STRATE_INNER_NAMESPACE(material)

class ShaderPass {
 public:
  explicit ShaderPass(ShaderEffect* effect): effect_(effect) {};

  void* GetProgram() const { return effect_->GetProgram(); }

 private:
  ShaderEffect* effect_;
};

STRATE_INNER_END_NAMESPACE