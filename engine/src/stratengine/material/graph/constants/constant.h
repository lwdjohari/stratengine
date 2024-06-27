#include <glm/glm.hpp>

#include "stratengine/material/graph/graph_base.h"

STRATE_INNER_NAMESPACE(material)
STRATE_MAKE_NAMESPACE(graph)
STRATE_MAKE_NAMESPACE(constants)

class Constant1Vector final : public GraphBase {
 public:
  Constant1Vector()
                  : GraphBase(NodeGroup::kConstant, NodeType::kConstant),
                    value_() {}

  explicit Constant1Vector(const glm::vec1& value)
                  : GraphBase(NodeGroup::kConstant, NodeType::kConstant),
                    value_(glm::vec1(value)) {}

  explicit Constant1Vector(glm::vec1&& value)
                  : GraphBase(NodeGroup::kConstant, NodeType::kConstant),
                    value_(std::forward<glm::vec1>(value)) {}

  Constant1Vector(const Constant1Vector& other)
                  : GraphBase(NodeGroup::kConstant, NodeType::kConstant),
                    value_(glm::vec1(other.value_)) {}

  virtual ~Constant1Vector(){};

  void SetValue(glm::vec1 value) {
    value_ = value;
  }

  glm::vec1& Value() {
    return value_;
  }

  const glm::vec1& Value() const {
    return value_;
  }

  // Compound assignment operators

  Constant1Vector& operator+=(const Constant1Vector& other) {
    this->value_ += other.value_;
    return *this;
  }

  Constant1Vector& operator-=(const Constant1Vector& other) {
    this->value_ -= other.value_;
    return *this;
  }

  Constant1Vector& operator*=(const Constant1Vector& other) {
    this->value_ *= other.value_;
    return *this;
  }

  Constant1Vector& operator/=(const Constant1Vector& other) {
    this->value_ /= other.value_;
    return *this;
  }

  // Increment and decrement operators
  Constant1Vector& operator++() {  // Prefix increment
    ++this->value_;
    return *this;
  }

  Constant1Vector operator++(int) {  // Postfix decrement
    Constant1Vector temp = *this;
    ++(*this);
    return temp;
  }

  Constant1Vector& operator--() {  // Prefix decrement
    --this->value_;
    return *this;
  }

  Constant1Vector operator--(int) {  // Postfix decrement
    Constant1Vector temp = *this;
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

  Constant1Vector operator+(const Constant1Vector& other) const {
    auto res = this->value_ + other.value_;
    return Constant1Vector(std::move(res));
  }

  Constant1Vector operator-(const Constant1Vector& other) const {
    auto res = this->value_ - other.value_;
    return Constant1Vector(std::move(res));
  }

  Constant1Vector operator*(const Constant1Vector& other) const {
    auto res = this->value_ * other.value_;
    return Constant1Vector(std::move(res));
  }

  Constant1Vector operator/(const Constant1Vector& other) const {
    auto res = this->value_ / other.value_;
    return Constant1Vector(std::move(res));
  }

  // Boolean operators
  bool operator==(const Constant1Vector& other) const {
    return this->value_ == other.value_;
  }

  bool operator!=(const Constant1Vector& other) const {
    return !(*this == other);
  }

 private:
  glm::vec1 value_;
};

STRATE_END_NAMESPACE
STRATE_END_NAMESPACE
STRATE_INNER_END_NAMESPACE