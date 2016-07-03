#pragma once

#include <vector>

#include "Segment.hpp"

namespace Tamarack {
  namespace Model {
    class Branch final {
    public:
      Branch() = default;
      Branch(const std::vector<Segment>& body);
      Branch(const std::vector<Segment>& body, const std::vector<Branch>& branches);
      Branch(const Branch&) = default;
      Branch(Branch&&) = default;

      const std::vector<Segment>& body() const;
      const std::vector<Branch>& branches() const;

      void translate(const Vector2d& delta);
    private:
      std::vector<Segment> _body;
      std::vector<Branch> _branches;
    };
  }
}