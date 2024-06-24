#include "stratengine/scene.h"

STRATE_BEGIN_NAMESPACE

void Scene::AddGameObject(std::unique_ptr<GameObject> game_object) {
  game_objects_.push_back(std::move(game_object));
}

void Scene::AddCamera(std::unique_ptr<component::CameraComponent> camera) {
  if (!active_camera_) {
    active_camera_ = camera.get();
  }
  cameras_.push_back(std::move(camera));
}

void Scene::AddLight(std::unique_ptr<component::LightComponent> light) {
  lights_.push_back(std::move(light));
}

void Scene::Update(double delta_time) {
  for (auto& game_object : game_objects_) {
    game_object->Update(delta_time);
  }
  for (auto& camera : cameras_) {
    camera->Update(delta_time);
  }
  for (auto& light : lights_) {
    light->Update(delta_time);
  }
}

void Scene::Render() {
  for (auto& game_object : game_objects_) {
    game_object->Render();
  }
}

component::CameraComponent* Scene::GetActiveCamera() const {
  return active_camera_;
}

const std::vector<std::unique_ptr<component::LightComponent>>&
Scene::GetLights() const {
  return lights_;
}

STRATE_END_NAMESPACE