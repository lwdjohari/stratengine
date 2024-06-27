#include <glm/glm.hpp>

#include "stratengine/material/graph/graph_base.h"

STRATE_INNER_NAMESPACE(material)
STRATE_MAKE_NAMESPACE(graph)
STRATE_MAKE_NAMESPACE(constants)

class Constant2Vector final : public GraphBase {
 public:
  Constant2Vector()
                  : GraphBase(NodeGroup::kConstant, NodeType::kConstant2Vector),
                    value_() {}

  explicit Constant2Vector(const glm::vec2& value)
                  : GraphBase(NodeGroup::kConstant, NodeType::kConstant2Vector),
                    value_(glm::vec2(value)) {}

  explicit Constant2Vector(glm::vec2&& value)
                  : GraphBase(NodeGroup::kConstant, NodeType::kConstant2Vector),
                    value_(std::forward<glm::vec2>(value)) {}

  Constant2Vector(const Constant2Vector& other)
                  : GraphBase(NodeGroup::kConstant, NodeType::kConstant2Vector),
                    value_(glm::vec2(other.value_)) {}

  virtual ~Constant2Vector(){};

  void SetValue(glm::vec2 value) {
    value_ = value;
  }

  glm::vec2& Value() {
    return value_;
  }

  const glm::vec2& Value() const {
    return value_;
  }

  // Compound assignment operators

  Constant2Vector& operator+=(const Constant2Vector& other) {
    this->value_ += other.value_;
    return *this;
  }

  Constant2Vector& operator-=(const Constant2Vector& other) {
    this->value_ -= other.value_;
    return *this;
  }

  Constant2Vector& operator*=(const Constant2Vector& other) {
    this->value_ *= other.value_;
    return *this;
  }

  Constant2Vector& operator/=(const Constant2Vector& other) {
    this->value_ /= other.value_;
    return *this;
  }

  // Increment and decrement operators
  Constant2Vector& operator++() {  // Prefix increment
    ++this->value_;
    return *this;
  }

  Constant2Vector operator++(int) {  // Postfix decrement
    Constant2Vector temp = *this;
    ++(*this);
    return temp;
  }

  Constant2Vector& operator--() {  // Prefix decrement
    --this->value_;
    return *this;
  }

  Constant2Vector operator--(int) {  // Postfix decrement
    Constant2Vector temp = *this;
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

  Constant2Vector operator+(const Constant2Vector& other) const {
    auto res = this->value_ + other.value_;
    return Constant2Vector(std::move(res));
  }

  Constant2Vector operator-(const Constant2Vector& other) const {
    auto res = this->value_ - other.value_;
    return Constant2Vector(std::move(res));
  }

  Constant2Vector operator*(const Constant2Vector& other) const {
    auto res = this->value_ * other.value_;
    return Constant2Vector(std::move(res));
  }

  Constant2Vector operator/(const Constant2Vector& other) const {
    auto res = this->value_ / other.value_;
    return Constant2Vector(std::move(res));
  }

  // Boolean operators
  bool operator==(const Constant2Vector& other) const {
    return this->value_ == other.value_;
  }

  bool operator!=(const Constant2Vector& other) const {
    return !(*this == other);
  }

 private:
  glm::vec2 value_;
};

STRATE_END_NAMESPACE
STRATE_END_NAMESPACE
STRATE_INNER_END_NAMESPACE