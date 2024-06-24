#pragma once
#include <bgfx/bgfx.h>

#include <glm/glm.hpp>
#include <unordered_map>
#include <vector>

#include "stratengine/macro.h"
#include "stratengine/material/effect_template.h"

// cppcheck-suppress unknownMacro
STRATE_INNER_NAMESPACE(material)


enum class TextureType {
    Diffuse,
    Specular,
    Normal,
    Emissive,
    AmbientOcclusion,
    Roughness,
    Metallic,
    Height,
    Opacity,
    Lightmap,
    // Add more texture types as needed
};

struct SampledTexture {
    bgfx::TextureHandle texture_handle;
    bgfx::UniformHandle uniform_handle;
};

class Material {
 public:
  Material(EffectTemplate* effect_template);

  void SetDiffuseColor(const glm::vec3& color);
  void SetSpecularColor(const glm::vec3& color);
  void SetAmbientColor(const glm::vec3& color);
  void SetEmissiveColor(const glm::vec3& color);
  void SetTransparency(float transparency);
  void SetShininess(float shininess);

  void SetTexture(TextureType type, const SampledTexture& texture);
  void Apply(PassType pass_type);
  bgfx::ProgramHandle GetProgram() const;

  void EnableFeature(TextureType type, bool enable);

 private:
  EffectTemplate* effect_template_;
  std::unordered_map<TextureType, SampledTexture> textures_;

  glm::vec3 diffuse_color_;
  glm::vec3 specular_color_;
  glm::vec3 ambient_color_;
  glm::vec3 emissive_color_;
  float transparency_;
  float shininess_;

  std::unordered_map<TextureType, bool> feature_enabled_;
};


STRATE_INNER_END_NAMESPACE