#pragma once

#include <bgfx/bgfx.h>
#include <vector>
#include <string>
#include "stratengine/macro.h"

STRATE_INNER_NAMESPACE(material)

struct ShaderStage {
    bgfx::ShaderHandle shader_handle;
    std::string type;
};

class ShaderEffect {
 public:
  void AddStage(const std::string& shader_path, const std::string& type);
  void Build();

  bgfx::ProgramHandle GetProgram() const;

 private:
  std::vector<ShaderStage> stages_;
  bgfx::ProgramHandle program_;
};

STRATE_INNER_END_NAMESPACE