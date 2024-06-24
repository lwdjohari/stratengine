#include "stratengine/material/material.h"

// cppcheck-suppress unknownMacro
STRATE_INNER_NAMESPACE(material)

Material::Material(EffectTemplate* effect_template)
                : effect_template_(effect_template),
                  diffuse_color_(1.0f),
                  specular_color_(1.0f),
                  ambient_color_(1.0f),
                  emissive_color_(0.0f),
                  transparency_(1.0f),
                  shininess_(32.0f) {
  for (const auto& type :
       {TextureType::Diffuse, TextureType::Specular, TextureType::Normal,
        TextureType::Emissive, TextureType::AmbientOcclusion,
        TextureType::Roughness, TextureType::Metallic, TextureType::Height,
        TextureType::Opacity, TextureType::Lightmap}) {
    feature_enabled_[type] = false;
  }
}

void Material::SetDiffuseColor(const glm::vec3& color) {
  diffuse_color_ = color;
}

void Material::SetSpecularColor(const glm::vec3& color) {
  specular_color_ = color;
}

void Material::SetAmbientColor(const glm::vec3& color) {
  ambient_color_ = color;
}

void Material::SetEmissiveColor(const glm::vec3& color) {
  emissive_color_ = color;
}

void Material::SetTransparency(float transparency) {
  transparency_ = transparency;
}

void Material::SetShininess(float shininess) {
  shininess_ = shininess;
}

void Material::SetTexture(TextureType type, const SampledTexture& texture) {
  textures_[type] = texture;
}

void Material::Apply(PassType pass_type) {
  auto pass = effect_template_->GetPass(pass_type);
  for (const auto& [type, texture] : textures_) {
    if (feature_enabled_[type]) {
      bgfx::setTexture(static_cast<uint8_t>(type), texture.uniform_handle,
                       texture.texture_handle);
    }
  }

  bgfx::setUniform(
      bgfx::createUniform("u_diffuseColor", bgfx::UniformType::Vec4),
      &diffuse_color_);
  bgfx::setUniform(
      bgfx::createUniform("u_specularColor", bgfx::UniformType::Vec4),
      &specular_color_);
  bgfx::setUniform(
      bgfx::createUniform("u_ambientColor", bgfx::UniformType::Vec4),
      &ambient_color_);
  bgfx::setUniform(
      bgfx::createUniform("u_emissiveColor", bgfx::UniformType::Vec4),
      &emissive_color_);
  bgfx::setUniform(
      bgfx::createUniform("u_transparency", bgfx::UniformType::Vec4),
      &transparency_);
  bgfx::setUniform(bgfx::createUniform("u_shininess", bgfx::UniformType::Vec4),
                   &shininess_);

  for (const auto& [type, enabled] : feature_enabled_) {
    std::string uniform_name =
        "u_enable" + std::to_string(static_cast<int>(type));
    int enabled_int = enabled ? 1 : 0;
    bgfx::setUniform(
        bgfx::createUniform(uniform_name.c_str(), bgfx::UniformType::Vec4),
        &enabled_int);
  }

  bgfx::submit(0, pass->GetProgram());
}

void Material::EnableFeature(TextureType type, bool enable) {
  feature_enabled_[type] = enable;
}

STRATE_INNER_END_NAMESPACE