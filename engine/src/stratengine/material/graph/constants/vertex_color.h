#pragma once

#include <glm/glm.hpp>

#include "stratengine/material/graph/graph_base.h"

STRATE_INNER_NAMESPACE(material)
STRATE_MAKE_NAMESPACE(graph)
STRATE_MAKE_NAMESPACE(constants)

class VertexColor final : public GraphBase {
 public:
  VertexColor()
                  : GraphBase(NodeGroup::kConstant, NodeType::kVertexColor),
                    color_() {}

  explicit VertexColor(const glm::vec4& color)
                  : GraphBase(NodeGroup::kConstant, NodeType::kVertexColor),
                    color_(color) {}

  explicit VertexColor(const float& r, const float& g, const float& b,
                       const float& a)
                  : GraphBase(NodeGroup::kConstant, NodeType::kVertexColor),
                    color_(glm::vec4(r, g, b, a)) {}

  explicit VertexColor(glm::vec4&& color)
                  : GraphBase(NodeGroup::kConstant, NodeType::kVertexColor),
                    color_(std::forward<glm::vec4>(color)) {}

  VertexColor(const VertexColor& other)
                  : GraphBase(NodeGroup::kConstant, NodeType::kVertexColor),
                    color_(other.color_) {}

  virtual ~VertexColor() = default;

  void SetColor(const glm::vec4& color) {
    color_ = color;
  }

  void SetColor(const float& r, const float& g, const float& b,
                const float& a) {
    color_.r = float(r);
    color_.g = float(g);
    color_.b = float(b);
    color_.a = float(a);
  }

  void SetR(const float& r) {
    color_.r = float(r);
  }

  void SetG(const float& g) {
    color_.g = float(g);
  }

  void SetB(const float& b) {
    color_.b = float(b);
  }

  void SetA(const float& a) {
    color_.a = float(a);
  }

  float& R() {
    return color_.r;
  }

  float& G() {
    return color_.g;
  }

  float& B() {
    return color_.b;
  }

  float& A() {
    return color_.a;
  }

  const float& R() const {
    return color_.r;
  }

  const float& G() const {
    return color_.g;
  }

  const float& B() const {
    return color_.b;
  }

  const float& A() const {
    return color_.a;
  }

  glm::vec4& Color() {
    return color_;
  }

  const glm::vec4& Color() const {
    return color_;
  }

  // Compound assignment operators
  VertexColor& operator+=(const VertexColor& other) {
    this->color_ += other.color_;
    return *this;
  }

  VertexColor& operator-=(const VertexColor& other) {
    this->color_ -= other.color_;
    return *this;
  }

  VertexColor& operator*=(const VertexColor& other) {
    this->color_ *= other.color_;
    return *this;
  }

  VertexColor& operator/=(const VertexColor& other) {
    this->color_ /= other.color_;
    return *this;
  }

  // Increment and decrement operators
  VertexColor& operator++() {  // Prefix increment
    ++this->color_.r;
    ++this->color_.g;
    ++this->color_.b;
    ++this->color_.a;
    return *this;
  }

  VertexColor operator++(int) {  // Postfix increment
    VertexColor temp = *this;
    ++(*this);
    return temp;
  }

  VertexColor& operator--() {  // Prefix decrement
    --this->color_.r;
    --this->color_.g;
    --this->color_.b;
    --this->color_.a;
    return *this;
  }

  VertexColor operator--(int) {  // Postfix decrement
    VertexColor temp = *this;
    --(*this);
    return temp;
  }

  // Subscript operator
  float& operator[](size_t index) {
    return color_[index];
  }

  const float& operator[](size_t index) const {
    return color_[index];
  }

  // Mathematical operators
  VertexColor operator+(const VertexColor& other) const {
    return VertexColor(this->color_ + other.color_);
  }

  VertexColor operator-(const VertexColor& other) const {
    return VertexColor(this->color_ - other.color_);
  }

  VertexColor operator*(const VertexColor& other) const {
    return VertexColor(this->color_ * other.color_);
  }

  VertexColor operator/(const VertexColor& other) const {
    return VertexColor(this->color_ / other.color_);
  }

  // Boolean operators
  bool operator==(const VertexColor& other) const {
    return this->color_ == other.color_;
  }

  bool operator!=(const VertexColor& other) const {
    return !(*this == other);
  }

 private:
  glm::vec4 color_;
};

STRATE_END_NAMESPACE
STRATE_END_NAMESPACE
STRATE_INNER_END_NAMESPACE