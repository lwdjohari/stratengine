#include <glm/glm.hpp>

#include "stratengine/material/graph/graph_base.h"

STRATE_INNER_NAMESPACE(material)
STRATE_MAKE_NAMESPACE(graph)
STRATE_MAKE_NAMESPACE(constants)

class Constant3Vector final : public GraphBase {
 public:
  Constant3Vector()
                  : GraphBase(NodeGroup::kConstant, NodeType::kConstant3Vector),
                    value_() {}

  explicit Constant3Vector(const glm::vec3& value)
                  : GraphBase(NodeGroup::kConstant, NodeType::kConstant3Vector),
                    value_(glm::vec3(value)) {}

  explicit Constant3Vector(glm::vec3&& value)
                  : GraphBase(NodeGroup::kConstant, NodeType::kConstant3Vector),
                    value_(std::forward<glm::vec3>(value)) {}

  Constant3Vector(const Constant3Vector& other)
                  : GraphBase(NodeGroup::kConstant, NodeType::kConstant3Vector),
                    value_(glm::vec3(other.value_)) {}

  virtual ~Constant3Vector(){};

  void SetValue(glm::vec3 value) {
    value_ = value;
  }

  glm::vec3& Value() {
    return value_;
  }

  const glm::vec3& Value() const {
    return value_;
  }

  // Compound assignment operators

  Constant3Vector& operator+=(const Constant3Vector& other) {
    this->value_ += other.value_;
    return *this;
  }

  Constant3Vector& operator-=(const Constant3Vector& other) {
    this->value_ -= other.value_;
    return *this;
  }

  Constant3Vector& operator*=(const Constant3Vector& other) {
    this->value_ *= other.value_;
    return *this;
  }

  Constant3Vector& operator/=(const Constant3Vector& other) {
    this->value_ /= other.value_;
    return *this;
  }

  // Increment and decrement operators
  Constant3Vector& operator++() {  // Prefix increment
    ++this->value_;
    return *this;
  }

  Constant3Vector operator++(int) {  // Postfix decrement
    Constant3Vector temp = *this;
    ++(*this);
    return temp;
  }

  Constant3Vector& operator--() {  // Prefix decrement
    --this->value_;
    return *this;
  }

  Constant3Vector operator--(int) {  // Postfix decrement
    Constant3Vector temp = *this;
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

  Constant3Vector operator+(const Constant3Vector& other) const {
    auto res = this->value_ + other.value_;
    return Constant3Vector(std::move(res));
  }

  Constant3Vector operator-(const Constant3Vector& other) const {
    auto res = this->value_ - other.value_;
    return Constant3Vector(std::move(res));
  }

  Constant3Vector operator*(const Constant3Vector& other) const {
    auto res = this->value_ * other.value_;
    return Constant3Vector(std::move(res));
  }

  Constant3Vector operator/(const Constant3Vector& other) const {
    auto res = this->value_ / other.value_;
    return Constant3Vector(std::move(res));
  }

  // Boolean operators
  bool operator==(const Constant3Vector& other) const {
    return this->value_ == other.value_;
  }

  bool operator!=(const Constant3Vector& other) const {
    return !(*this == other);
  }

 private:
  glm::vec3 value_;
};

STRATE_END_NAMESPACE
STRATE_END_NAMESPACE
STRATE_INNER_END_NAMESPACE