#pragma once

#include "PropertyHelper.hpp"
#include "Vector2d.hpp"

namespace Tamarak {
namespace Model {
class Segment final {
 public:
  Segment(const Vector2d& position, const Vector2d& direction, float width);
  Segment(Segment&&) = default;
  Segment(const Segment&) = default;

  Segment& operator=(const Segment&) = default;

  PROP_SET_AND_GET(Vector2d, position);
  PROP_SET_AND_GET(Vector2d, direction);
  PROP_SET_AND_GET(float, width);

  void translate(const Vector2d& delta);
};
}
}