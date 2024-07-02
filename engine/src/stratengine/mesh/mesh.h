#pragma once


#include <glm/glm.hpp>
#include <vector>

#include "stratengine/macro.h"
#include "stratengine/material/layered_material.h"
#include "stratengine/material/material.h"

STRATE_INNER_NAMESPACE(mesh)

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 tex_coords;
};

class Mesh {
 public:
  Mesh(const std::vector<Vertex>& vertices,
       const std::vector<uint32_t>& indices);

  // void Render(bgfx::ProgramHandle shader_program);

  void* GetVertexBuffer() const;
  void* GetIndexBuffer() const;
  void SetMaterial(material::Material* material);
  void SetLayeredMaterial(material::LayeredMaterial* layeredMaterial);
  void SetVertices(const std::vector<glm::vec3>& vertices);
  void SetIndices(const std::vector<uint16_t>& indices);
  void Render(material::PassType pass_type);

 private:
  material::Material* material_;
  material::LayeredMaterial* layeredMaterial_;
  void* vbh_;
  void* ibh_;
  std::vector<glm::vec3> vertices_;
  std::vector<uint16_t> indices_;
};

STRATE_INNER_END_NAMESPACE