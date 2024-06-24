#pragma once

#include <string>

#include "stratengine/asset/asset_manager.h"
#include "stratengine/game_object.h"
#include "stratengine/material/material.h"
#include "stratengine/component/transform_component.h"
#include "stratengine/component/camera_component.h"
#include "stratengine/component/light_component.h"
#include "stratengine/component/mesh_component.h"

#include "stratengine/scene.h"

STRATE_BEGIN_NAMESPACE

class SceneLoader {
 public:
  std::unique_ptr<Scene> LoadScene(const std::string& scene_path);

 private:
  void ParseSceneFile(const std::string& scene_path, Scene* scene);
  std::unique_ptr<GameObject> CreateGameObjectFromAsset(
      const std::string& asset_path);
};

STRATE_END_NAMESPACE