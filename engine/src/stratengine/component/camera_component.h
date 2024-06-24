#pragma once

#include "stratengine/component/component.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

STRATE_INNER_NAMESPACE(component)

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "component.h"

class CameraComponent : public Component {
 public:
  explicit CameraComponent(float fov, float aspect_ratio, float near_plane,
                  float far_plane)
                  : fov_(fov),
                    aspect_ratio_(aspect_ratio),
                    near_plane_(near_plane),
                    far_plane_(far_plane) {}

  void Update(double delta_time) override {}
  void Render() override {}

  glm::mat4 GetViewMatrix() const {
    return glm::lookAt(position_, position_ + front_, up_);
  }

  glm::mat4 GetProjectionMatrix() const {
    return glm::perspective(glm::radians(fov_), aspect_ratio_, near_plane_,
                            far_plane_);
  }

  void SetPosition(const glm::vec3& position) {
    position_ = position;
  }
  void SetFront(const glm::vec3& front) {
    front_ = front;
  }
  void SetUp(const glm::vec3& up) {
    up_ = up;
  }

 private:
  glm::vec3 position_{0.0f, 0.0f, 3.0f};
  glm::vec3 front_{0.0f, 0.0f, -1.0f};
  glm::vec3 up_{0.0f, 1.0f, 0.0f};
  float fov_;
  float aspect_ratio_;
  float near_plane_;
  float far_plane_;
};

STRATE_INNER_END_NAMESPACE