#pragma once

#include <vector>

#include "PropertyHelper.hpp"
#include "Segment.hpp"

namespace Tamarak {
namespace Model {
class Trunk final {
 public:
  Trunk() = default;
  Trunk(const std::vector<Segment>& body);
  Trunk(const Trunk&) = default;
  Trunk(Trunk&&) = default;

  Trunk& operator=(const Trunk&) = default;

  bool operator==(const Trunk&) const;
  bool operator!=(const Trunk&) const;

  PROP_SET_AND_GET(std::vector<Segment>, body);
};
}
}