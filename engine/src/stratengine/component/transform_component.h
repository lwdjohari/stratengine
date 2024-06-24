#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "stratengine/component/component.h"
#include "stratengine/component/transform_component.h"
STRATE_INNER_NAMESPACE(component)

class TransformComponent : public Component {
 public:
  glm::vec3 position;
  glm::vec3 rotation;
  glm::vec3 scale;

  TransformComponent() : position(0.0f), rotation(0.0f), scale(1.0f) {}

  void Update(double delta_time) override {}
  void Render() override {}

  glm::mat4 GetLocalTransformMatrix() const {
    glm::mat4 trans = glm::translate(glm::mat4(1.0f), position);
    trans = glm::rotate(trans, glm::radians(rotation.x), glm::vec3(1, 0, 0));
    trans = glm::rotate(trans, glm::radians(rotation.y), glm::vec3(0, 1, 0));
    trans = glm::rotate(trans, glm::radians(rotation.z), glm::vec3(0, 0, 1));
    trans = glm::scale(trans, scale);
    return trans;
  }

  glm::mat4 GetWorldTransformMatrix() const {
    if (game_object_->GetParent() != nullptr) {
      auto parent_transform =
          game_object_->GetParent()->GetComponent<TransformComponent>();
      return parent_transform->GetWorldTransformMatrix() *
             GetLocalTransformMatrix();
    }
    return GetLocalTransformMatrix();
  }

  
};

STRATE_INNER_END_NAMESPACE