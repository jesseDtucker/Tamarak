#pragma once

#include <vector>

#include "Branch.hpp"
#include "PropertyHelper.hpp"
#include "Trunk.hpp"

namespace Tamarak {
namespace Model {
class Tree final {
 public:
  Tree() = default;
  Tree(const Trunk& trunk, const std::vector<Branch>& branches);
  Tree(const Tree&) = default;
  Tree(Tree&&) = default;

  Tree& operator=(const Tree&) = default;

  bool operator==(const Tree&) const;
  bool operator!=(const Tree&) const;

  PROP_SET_AND_GET(Trunk, trunk);
  PROP_SET_AND_GET(std::vector<Branch>, branches);
};
}
}