#pragma once

#include "stratengine/game_object.h"
#include "stratengine/macro.h"

STRATE_INNER_NAMESPACE(component)
  
class Component {
 public:
  virtual ~Component() = default;
  virtual void Update(double delta_time) = 0;
  virtual void Render() = 0;

  void SetGameObject(GameObject* game_object) {
    game_object_ = game_object;
  }

 protected:
  GameObject* game_object_ = nullptr;
};

STRATE_INNER_END_NAMESPACE

// cppcheck-suppress unknownMacro
STRATE_BEGIN_NAMESPACE

template <typename T, typename... Args>
  T* GameObject::AddComponent(Args&&... args) {
    static_assert(std::is_base_of<component::Component, T>::value,
                  "T must derive from Component");
    T* component = new T(std::forward<Args>(args)...);
    component->SetGameObject(this);
    components_.emplace_back(component);
    return component;
  }

  template <typename T>
  T* GameObject::GetComponent() {
    for (auto& component : components_) {
      if (auto comp = dynamic_cast<T*>(component)) {
        return comp;
      }
    }
    return nullptr;
  }

  void GameObject::Update(double deltaTime) {
    for (auto& component : components_) {
      component->Update(deltaTime);
    }
  }

  void GameObject::Render() {
    for (auto& component : components_) {
      component->Render();
    }
  }

  void GameObject::SetParent(GameObject* parent) {
    parent_ = parent;
  }

  GameObject* GameObject::GetParent() const {
    return parent_;
  }

  void GameObject::AddChild(GameObject* child) {
    child->SetParent(this);
    children_.push_back(child);
  }

GameObject::~GameObject() {
    for (auto& component : components_) {
      delete component;
    }
  }

STRATE_END_NAMESPACE