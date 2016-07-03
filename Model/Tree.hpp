#pragma once

#include <vector>

#include "Branch.hpp"
#include "Trunk.hpp"

namespace Tamarack {
  namespace Model {
    class Tree {
    public:
      Tree() = default;
      Tree(const Trunk& trunk, const std::vector<Branch>& branches);
      Tree(const Tree&) = default;
      Tree(Tree&&) = default;

      const Trunk trunk() const;
      const std::vector<Branch> branches() const;
    private:
      Trunk _trunk;
      std::vector<Branch> _branches;
    };
  }
}