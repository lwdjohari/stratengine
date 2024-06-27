#include "stratengine/material/graph/graph_base.h"

#include <sstream>
// cppcheck-suppress unknownMacro
STRATE_INNER_NAMESPACE(material)
STRATE_MAKE_NAMESPACE(graph)

GraphBase::GraphBase(NodeGroup group, NodeType type)
                : group_(group), type_(type) {}

GraphBase::~GraphBase() {}

// const size_t& GraphBase::Key() const {
//   return key_;
// }

// const std::string& GraphBase::Name() const {
//   return name_;
// }

const NodeGroup& GraphBase::Group() const {
  return group_;
}

const NodeType& GraphBase::Type() const {
  return type_;
}

// ----------- Protected ---------

// std::string GraphBase::CreateRandomizeKey(const std::string& prefix) const {
//   size_t random = utility::random::RandomizeUint64t();
//   auto timestamp = std::chrono::steady_clock::now();
//   auto tp = std::chrono::duration_cast<std::chrono::milliseconds>(
//                 timestamp.time_since_epoch())
//                 .count();
//   std::ostringstream ss;
//   ss << prefix << "." << tp << "." << random;
//   return ss.str();
// }

// // ----------- Private -----------
// size_t GraphBase::FromStringToHash(const std::string& key) {
//   std::hash<std::string> hash;
//   return hash(key);
// }

STRATE_END_NAMESPACE
STRATE_INNER_END_NAMESPACE