#include <glm/glm.hpp>

#include "stratengine/material/graph/graph_base.h"

STRATE_INNER_NAMESPACE(material)
STRATE_MAKE_NAMESPACE(graph)
STRATE_MAKE_NAMESPACE(constants)

class Constant4Vector final : public GraphBase {
 public:
  Constant4Vector()
                  : GraphBase(NodeGroup::kConstant, NodeType::kConstant4Vector),
                    value_() {}

  explicit Constant4Vector(const glm::vec4& value)
                  : GraphBase(NodeGroup::kConstant, NodeType::kConstant4Vector),
                    value_(glm::vec4(value)) {}

  explicit Constant4Vector(glm::vec4&& value)
                  : GraphBase(NodeGroup::kConstant, NodeType::kConstant4Vector),
                    value_(std::forward<glm::vec4>(value)) {}

  Constant4Vector(const Constant4Vector& other)
                  : GraphBase(NodeGroup::kConstant, NodeType::kConstant4Vector),
                    value_(glm::vec4(other.value_)) {}

  virtual ~Constant4Vector(){};

  void SetValue(glm::vec4 value) {
    value_ = value;
  }

  glm::vec4& Value() {
    return value_;
  }

  const glm::vec4& Value() const {
    return value_;
  }

  // Compound assignment operators

  Constant4Vector& operator+=(const Constant4Vector& other) {
    this->value_ += other.value_;
    return *this;
  }

  Constant4Vector& operator-=(const Constant4Vector& other) {
    this->value_ -= other.value_;
    return *this;
  }

  Constant4Vector& operator*=(const Constant4Vector& other) {
    this->value_ *= other.value_;
    return *this;
  }

  Constant4Vector& operator/=(const Constant4Vector& other) {
    this->value_ /= other.value_;
    return *this;
  }

  // Increment and decrement operators
  Constant4Vector& operator++() {  // Prefix increment
    ++this->value_;
    return *this;
  }

  Constant4Vector operator++(int) {  // Postfix decrement
    Constant4Vector temp = *this;
    ++(*this);
    return temp;
  }

  Constant4Vector& operator--() {  // Prefix decrement
    --this->value_;
    return *this;
  }

  Constant4Vector operator--(int) {  // Postfix decrement
    Constant4Vector temp = *this;
    --(*this);
    return temp;
  }

  // Subscript operator
  float& operator[](size_t index) {
    return this->value_[index];
  }

  const float& operator[](size_t index) const {
    return value_[index];
  }

  // Mathematical operators

  Constant4Vector operator+(const Constant4Vector& other) const {
    auto res = this->value_ + other.value_;
    return Constant4Vector(std::move(res));
  }

  Constant4Vector operator-(const Constant4Vector& other) const {
    auto res = this->value_ - other.value_;
    return Constant4Vector(std::move(res));
  }

  Constant4Vector operator*(const Constant4Vector& other) const {
    auto res = this->value_ * other.value_;
    return Constant4Vector(std::move(res));
  }

  Constant4Vector operator/(const Constant4Vector& other) const {
    auto res = this->value_ / other.value_;
    return Constant4Vector(std::move(res));
  }

  // Boolean operators
  bool operator==(const Constant4Vector& other) const {
    return this->value_ == other.value_;
  }

  bool operator!=(const Constant4Vector& other) const {
    return !(*this == other);
  }

 private:
  glm::vec4 value_;
};

STRATE_END_NAMESPACE
STRATE_END_NAMESPACE
STRATE_INNER_END_NAMESPACE