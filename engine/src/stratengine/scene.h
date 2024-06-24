#pragma once

#include <memory>
#include <vector>

#include "stratengine/component/camera_component.h"
#include "stratengine/component/light_component.h"
#include "stratengine/game_object.h"
#include "stratengine/macro.h"

STRATE_BEGIN_NAMESPACE

class Scene {
 public:
  Scene() = default;
  ~Scene() = default;

  void AddGameObject(std::unique_ptr<GameObject> game_object);
  void AddCamera(std::unique_ptr<component::CameraComponent> camera);
  void AddLight(std::unique_ptr<component::LightComponent> light);

  void Update(double delta_time);
  void Render();

  component::CameraComponent* GetActiveCamera() const;
  const std::vector<std::unique_ptr<component::LightComponent>>& GetLights() const;

 private:
  std::vector<std::unique_ptr<GameObject>> game_objects_;
  std::vector<std::unique_ptr<component::CameraComponent>> cameras_;
  std::vector<std::unique_ptr<component::LightComponent>> lights_;
  component::CameraComponent* active_camera_ = nullptr;
};

STRATE_END_NAMESPACE