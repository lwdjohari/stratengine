#pragma once

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <memory>
#include <string>
#include <unordered_map>
#include <iostream>

#include "stratengine/macro.h"
#include "stratengine/mesh/mesh.h"

STRATE_INNER_NAMESPACE(asset)

class AssetManager {
 public:
  static AssetManager& GetInstance() {
    static AssetManager instance;
    return instance;
  }

  std::shared_ptr<mesh::Mesh> LoadModel(const std::string& path);

 private:
  AssetManager() = default;
  ~AssetManager() = default;
  AssetManager(const AssetManager&) = delete;
  AssetManager& operator=(const AssetManager&) = delete;

  std::shared_ptr<mesh::Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene);

  std::unordered_map<std::string, std::shared_ptr<mesh::Mesh>> mesh_cache_;
};

STRATE_INNER_END_NAMESPACE