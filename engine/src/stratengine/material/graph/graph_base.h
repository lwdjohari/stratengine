#pragma once

#include "stratengine/macro.h"
#include "stratengine/utility/random.h"

// cppcheck-suppress unknownMacro
STRATE_INNER_NAMESPACE(material)
STRATE_MAKE_NAMESPACE(graph)

enum class NodeGroup {
  kNone = 0,
  kConstant = 1,
  kTexture = 2,
  kCoordinate = 3
};

STRATE_ENUM_CLASS_DISPLAY_TRAIT(NodeGroup)

enum class NodeType {
  kNone = 0,
  kConstant = 1,
  kConstant2Vector = 2,
  kConstant3Vector = 4,
  kConstant4Vector = 8,
  kConstantDouble = 16,
  kConstantDeltaTime = 32,
  kConstantTime = 64,
  kVertexColor = 128,
  kColorConstant = 256,

};

STRATE_ENUM_CLASS_DISPLAY_TRAIT(NodeType)

STRATE_ENUM_CLASS_ENABLE_BITMASK_OPERATORS(NodeType)

class GraphBase {
 private:
 public:
  virtual ~GraphBase();

  const size_t& Key() const;
  const std::string& Name() const;
  const NodeGroup& Group() const;
  const NodeType& Type() const;

 protected:
  explicit GraphBase( NodeGroup group,
            NodeType type);

  // std::string CreateRandomizeKey(
  //     const std::string& prefix = std::string()) const;

 private:
  //size_t key_;
  //std::string name_;
  NodeGroup group_;
  NodeType type_;

  ///size_t FromStringToHash(const std::string& key);
};

STRATE_END_NAMESPACE
STRATE_INNER_END_NAMESPACE