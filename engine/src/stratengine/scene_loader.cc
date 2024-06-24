#include <fstream>
#include <iostream>
#include <sstream>

#include "stratengine/scene_loader.h"

STRATE_BEGIN_NAMESPACE
std::unique_ptr<Scene> SceneLoader::LoadScene(const std::string& scene_path) {
  auto scene = std::make_unique<Scene>();
  ParseSceneFile(scene_path, scene.get());
  return scene;
}

void SceneLoader::ParseSceneFile(const std::string& scene_path, Scene* scene) {
  std::ifstream file(scene_path);
  if (!file.is_open()) {
    std::cerr << "Failed to open scene file: " << scene_path << std::endl;
    return;
  }

  std::string line;
  while (std::getline(file, line)) {
    std::istringstream iss(line);
    std::string type, asset_path;
    if (!(iss >> type >> asset_path)) {
      continue;
    }

    if (type == "GameObject") {
      auto game_object = CreateGameObjectFromAsset(asset_path);
      if (game_object) {
        scene->AddGameObject(std::move(game_object));
      }
    } else if (type == "Camera") {
      auto camera =
          std::make_unique<component::CameraComponent>(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
      // Set camera properties based on asset_path (if needed)
      scene->AddCamera(std::move(camera));
    } else if (type == "Light") {
      auto light = std::make_unique<component::LightComponent>(component::LightType::POINT,
                                                    glm::vec3(1.0f), 1.0f);
      // Set light properties based on asset_path (if needed)
      scene->AddLight(std::move(light));
    }
  }
}

std::unique_ptr<GameObject> SceneLoader::CreateGameObjectFromAsset(
    const std::string& asset_path) {
  auto mesh = asset::AssetManager::GetInstance().LoadModel(asset_path);
  if (!mesh) {
    return nullptr;
  }

  auto game_object = std::make_unique<GameObject>();
  auto transform = game_object->AddComponent<component::TransformComponent>();
  auto material = std::make_unique<material::Material>(nullptr);
  auto mesh_component = game_object->AddComponent<component::MeshComponent>(
      mesh->GetVertexBuffer(), mesh->GetIndexBuffer(), material.get());

  return game_object;
}
STRATE_END_NAMESPACE
