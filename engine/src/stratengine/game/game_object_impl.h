#pragma once

#include <memory>
#include <type_traits>
#include <vector>

#include "stratengine/macro.h"

STRATE_BEGIN_NAMESPACE
namespace component {
class Component;
}

class GameObject {
 public:
  GameObject() {}
  ~GameObject();

  template <typename T, typename... Args>
  T* AddComponent(Args&&... args);

  template <typename T>
  T* GetComponent();

  void Update(double delta_time);

  void Render();

  void SetParent(GameObject* parent);

  GameObject* GetParent() const;

  void AddChild(GameObject* child);

 private:
  std::vector<component::Component*> components_;
  GameObject* parent_ = nullptr;
  std::vector<GameObject*> children_;
};

STRATE_END_NAMESPACE