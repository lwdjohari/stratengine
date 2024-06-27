#include <glm/glm.hpp>

#include "stratengine/material/graph/graph_base.h"

STRATE_INNER_NAMESPACE(material)
STRATE_MAKE_NAMESPACE(graph)
STRATE_MAKE_NAMESPACE(constants)

class ConstantDouble final : public GraphBase {
 public:
  ConstantDouble()
                  : GraphBase(NodeGroup::kConstant, NodeType::kConstantDouble),
                    value_() {}

  explicit ConstantDouble(const double& value)
                  : GraphBase(NodeGroup::kConstant, NodeType::kConstantDouble),
                    value_(double(value)) {}

  explicit ConstantDouble(double&& value)
                  : GraphBase(NodeGroup::kConstant, NodeType::kConstantDouble),
                    value_(std::forward<double>(value)) {}

  ConstantDouble(const ConstantDouble& other)
                  : GraphBase(NodeGroup::kConstant, NodeType::kConstantDouble),
                    value_(double(other.value_)) {}

  virtual ~ConstantDouble(){};

  void SetValue(double value) {
    value_ = value;
  }

  double& Value() {
    return value_;
  }

  const double& Value() const {
    return value_;
  }

  // Compound assignment operators

  ConstantDouble& operator+=(const ConstantDouble& other) {
    this->value_ += other.value_;
    return *this;
  }

  ConstantDouble& operator-=(const ConstantDouble& other) {
    this->value_ -= other.value_;
    return *this;
  }

  ConstantDouble& operator*=(const ConstantDouble& other) {
    this->value_ *= other.value_;
    return *this;
  }

  ConstantDouble& operator/=(const ConstantDouble& other) {
    this->value_ /= other.value_;
    return *this;
  }

  // Increment and decrement operators
  ConstantDouble& operator++() {  // Prefix increment
    ++this->value_;
    return *this;
  }

  ConstantDouble operator++(int) {  // Postfix decrement
    ConstantDouble temp = *this;
    ++(*this);
    return temp;
  }

  ConstantDouble& operator--() {  // Prefix decrement
    --this->value_;
    return *this;
  }

  ConstantDouble operator--(int) {  // Postfix decrement
    ConstantDouble temp = *this;
    --(*this);
    return temp;
  }

  
  // Mathematical operators

  ConstantDouble operator+(const ConstantDouble& other) const {
    auto res = this->value_ + other.value_;
    return ConstantDouble(std::move(res));
  }

  ConstantDouble operator-(const ConstantDouble& other) const {
    auto res = this->value_ - other.value_;
    return ConstantDouble(std::move(res));
  }

  ConstantDouble operator*(const ConstantDouble& other) const {
    auto res = this->value_ * other.value_;
    return ConstantDouble(std::move(res));
  }

  ConstantDouble operator/(const ConstantDouble& other) const {
    auto res = this->value_ / other.value_;
    return ConstantDouble(std::move(res));
  }

  // Boolean operators
  bool operator==(const ConstantDouble& other) const {
    return this->value_ == other.value_;
  }

  bool operator!=(const ConstantDouble& other) const {
    return !(*this == other);
  }

 private:
  double value_;
};

STRATE_END_NAMESPACE
STRATE_END_NAMESPACE
STRATE_INNER_END_NAMESPACE