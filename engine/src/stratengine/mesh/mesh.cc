#include "stratengine/mesh/mesh.h"

// cppcheck-suppress unknownMacro
STRATE_INNER_NAMESPACE(mesh)

Mesh::Mesh(const std::vector<Vertex>& vertices,
           const std::vector<uint32_t>& indices) {
  // Create vertex buffer
  bgfx::VertexLayout layout;
  layout.begin()
      .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
      .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
      .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
      .end();

  vbh_ = bgfx::createVertexBuffer(
      bgfx::makeRef(vertices.data(), vertices.size() * sizeof(Vertex)), layout);

  // Create index buffer
  ibh_ = bgfx::createIndexBuffer(
      bgfx::makeRef(indices.data(), indices.size() * sizeof(uint32_t)));

  // num_indices_ = static_cast<uint32_t>(indices.size());
}

bgfx::VertexBufferHandle Mesh::GetVertexBuffer() const {
  return vbh_;
}
bgfx::IndexBufferHandle Mesh::GetIndexBuffer() const {
  return ibh_;
}

void Mesh::SetMaterial(material::Material* material) {
  material_ = material;
}

void Mesh::SetLayeredMaterial(material::LayeredMaterial* layeredMaterial) {
  layeredMaterial_ = layeredMaterial;
}

void Mesh::SetVertices(const std::vector<glm::vec3>& vertices) {
  vertices_ = vertices;
  bgfx::VertexLayout layout;
  layout.begin().add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float).end();
  const bgfx::Memory* mem =
      bgfx::makeRef(vertices_.data(), vertices_.size() * sizeof(glm::vec3));
  vbh_ = bgfx::createVertexBuffer(mem, layout);
}

void Mesh::SetIndices(const std::vector<uint16_t>& indices) {
  indices_ = indices;
  const bgfx::Memory* mem =
      bgfx::makeRef(indices_.data(), indices_.size() * sizeof(uint16_t));
  ibh_ = bgfx::createIndexBuffer(mem);
}

void Mesh::Render(material::PassType pass_type) {
  bgfx::ProgramHandle program_handle;
  if (layeredMaterial_) {
    layeredMaterial_->Apply(pass_type);
    program_handle = layeredMaterial_->GetProgram();
  } else if (material_) {
    material_->Apply(pass_type);
    program_handle = material_->GetProgram();
  }
  bgfx::setVertexBuffer(0, vbh_);
  bgfx::setIndexBuffer(ibh_);
  bgfx::submit(0, program_handle);
}

STRATE_INNER_END_NAMESPACE