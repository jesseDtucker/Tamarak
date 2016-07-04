#pragma once

#include <vector>

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

  const std::vector<Segment> body() const;

 private:
  std::vector<Segment> _body;
};
}
}