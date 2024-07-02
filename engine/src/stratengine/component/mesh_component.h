#pragma once


#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stratengine/component/transform_component.h"
#include "stratengine/material/material.h"

STRATE_INNER_NAMESPACE(component)

class MeshComponent : public Component {
 public:
  MeshComponent(void* vbh, void* ibh,
                material::Material* material)
                  : vbh_(vbh), ibh_(ibh), material_(material) {}

  void Update(double delta_time) override {}

  void Render() override {
    if (game_object_) {
      // auto transform = game_object_->GetComponent<TransformComponent>();
      // if (transform) {
      //   glm::mat4 mtx = transform->GetWorldTransformMatrix();
      //   bgfx::setTransform(glm::value_ptr(mtx));

      //   bgfx::setVertexBuffer(0, vbh_);
      //   bgfx::setIndexBuffer(ibh_);

      //   material_->Apply(material::PassType::Forward);
      //   material_->Apply(material::PassType::Shadow);
        
      //   // material_->Apply();
      //   // bgfx::setVertexBuffer(0, vbh_);
      //   // bgfx::setIndexBuffer(ibh_);
      //   // bgfx::submit(0, material_->GetShaderProgram());
      // }
    }
  }

 private:
  void* vbh_;
  void* ibh_;
  material::Material* material_;
};


STRATE_INNER_END_NAMESPACE