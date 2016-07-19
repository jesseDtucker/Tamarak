#pragma once

#include <vector>

#include "Leaf.hpp"
#include "PropertyHelper.hpp"
#include "Segment.hpp"

namespace Tamarak {
namespace Model {
class Branch final {
 public:
  Branch() = default;
  Branch(const std::vector<Segment>& body);
  Branch(const std::vector<Segment>& body, const std::vector<Branch>& branches);
  Branch(const Branch&) = default;
  Branch(Branch&&) = default;

  Branch& operator=(const Branch&) = default;

  bool operator==(const Branch&) const;
  bool operator!=(const Branch&) const;

  PROP_SET_AND_GET(std::vector<Leaf>, leaves);
  PROP_SET_AND_GET(std::vector<Segment>, body);
  PROP_SET_AND_GET(std::vector<Branch>, branches);


  void translate(const Vector2d& delta);

 private:
};
}
}