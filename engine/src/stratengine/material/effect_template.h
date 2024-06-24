#include <unordered_map>

#include "stratengine/macro.h"
#include "stratengine/material/shader_pass.h"

// cppcheck-suppress unknownMacro
STRATE_INNER_NAMESPACE(material)

enum class PassType {
  Forward,
  Shadow,
  // Add more pass types as needed
};

class EffectTemplate {
 public:
  void AddPass(PassType pass_type, ShaderPass* pass);
  ShaderPass* GetPass(PassType pass_type);

 private:
  std::unordered_map<PassType, ShaderPass*> passes_;
};

STRATE_INNER_END_NAMESPACE
