#include "effect_template.h"

STRATE_INNER_NAMESPACE(material)

void EffectTemplate::AddPass(PassType pass_type, ShaderPass* pass) {
  passes_[pass_type] = pass;
}

ShaderPass* EffectTemplate::GetPass(PassType pass_type) {
  return passes_.at(pass_type);
}

STRATE_INNER_END_NAMESPACE