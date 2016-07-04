#pragma once

#include <vector>

#include "Branch.hpp"
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

  const Trunk trunk() const;
  const std::vector<Branch> branches() const;

 private:
  Trunk _trunk;
  std::vector<Branch> _branches;
};
}
}