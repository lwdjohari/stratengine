#pragma once

#include "stratengine/component/component.h"
#include <glm/glm.hpp>

// cppcheck-suppress unknownMacro
STRATE_INNER_NAMESPACE(component)

enum class LightType {
  DIRECTIONAL,
  POINT,
  SPOT
};

class LightComponent : public Component {
 public:
  LightComponent(LightType type, const glm::vec3& color, float intensity)
      : type_(type), color_(color), intensity_(intensity) {}

  void Update(double delta_time) override {}
  void Render() override {}

  LightType GetType() const { return type_; }
  glm::vec3 GetColor() const { return color_; }
  float GetIntensity() const { return intensity_; }

  void SetPosition(const glm::vec3& position) { position_ = position; }
  void SetDirection(const glm::vec3& direction) { direction_ = direction; }

  glm::vec3 GetPosition() const { return position_; }
  glm::vec3 GetDirection() const { return direction_; }

 private:
  LightType type_;
  glm::vec3 color_;
  float intensity_;
  glm::vec3 position_{0.0f};
  glm::vec3 direction_{0.0f, -1.0f, 0.0f};
};

STRATE_INNER_END_NAMESPACE