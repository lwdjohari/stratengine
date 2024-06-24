#include "stratengine/asset/asset_manager.h"
#include <iostream>

// cppcheck-suppress unknownMacro
STRATE_INNER_NAMESPACE(asset)


std::shared_ptr<mesh::Mesh> AssetManager::LoadModel(const std::string& path) {
  // Check if the mesh is already loaded
  if (mesh_cache_.find(path) != mesh_cache_.end()) {
    return mesh_cache_[path];
  }

  // Import the model using Assimp
  Assimp::Importer importer;
  const aiScene* scene =
      importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs |
                                  aiProcess_CalcTangentSpace);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !scene->mRootNode) {
    std::cerr << "Assimp error: " << importer.GetErrorString() << std::endl;
    return nullptr;
  }

  // Process the first mesh in the scene (for simplicity)
  std::shared_ptr<mesh::Mesh> mesh = ProcessMesh(scene->mMeshes[0], scene);
  mesh_cache_[path] = mesh;
  return mesh;
}

std::shared_ptr<mesh::Mesh> AssetManager::ProcessMesh(aiMesh* mesh,
                                                const aiScene* scene) {
  std::vector<mesh::Vertex> vertices;
  std::vector<uint32_t> indices;

  // Process vertices
  for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
    mesh::Vertex vertex;
    vertex.position = {mesh->mVertices[i].x, mesh->mVertices[i].y,
                       mesh->mVertices[i].z};
    vertex.normal = {mesh->mNormals[i].x, mesh->mNormals[i].y,
                     mesh->mNormals[i].z};
    if (mesh->mTextureCoords[0]) {
      vertex.tex_coords = {mesh->mTextureCoords[0][i].x,
                           mesh->mTextureCoords[0][i].y};
    } else {
      vertex.tex_coords = {0.0f, 0.0f};
    }
    vertices.push_back(vertex);
  }

  // Process indices
  for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
    aiFace face = mesh->mFaces[i];
    for (unsigned int j = 0; j < face.mNumIndices; ++j) {
      indices.push_back(face.mIndices[j]);
    }
  }

  return std::make_shared<mesh::Mesh>(vertices, indices);
}

STRATE_INNER_END_NAMESPACE