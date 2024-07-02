#include "stratengine/mesh/mesh.h"

// cppcheck-suppress unknownMacro
STRATE_INNER_NAMESPACE(mesh)

Mesh::Mesh(const std::vector<Vertex>& vertices,
           const std::vector<uint32_t>& indices) {
  // Create vertex buffer
  
}

void* Mesh::GetVertexBuffer() const {
  return vbh_;
}

void* Mesh::GetIndexBuffer() const {
  return ibh_;
}

void Mesh::SetMaterial(material::Material* material) {
  material_ = material;
}

void Mesh::SetLayeredMaterial(material::LayeredMaterial* layeredMaterial) {
  layeredMaterial_ = layeredMaterial;
}

void Mesh::SetVertices(const std::vector<glm::vec3>& vertices) {
  
}

void Mesh::SetIndices(const std::vector<uint16_t>& indices) {
 
}

void Mesh::Render(material::PassType pass_type) {
  
}

STRATE_INNER_END_NAMESPACE