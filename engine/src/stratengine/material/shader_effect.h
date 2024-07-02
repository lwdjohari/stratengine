#pragma once

#include <vector>
#include <string>
#include "stratengine/macro.h"

STRATE_INNER_NAMESPACE(material)

struct ShaderStage {
    void* shader_handle;
    std::string type;
};

class ShaderEffect {
 public:
  void AddStage(const std::string& shader_path, const std::string& type);
  void Build();

  void* GetProgram() const;

 private:
  std::vector<ShaderStage> stages_;
  void* program_;
};

STRATE_INNER_END_NAMESPACE