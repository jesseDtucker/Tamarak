#pragma once

#include "Vector2d.hpp"

namespace Tamarack {
namespace Model {
class Segment final {
 public:
  Segment(const Vector2d& position, const Vector2d& direction, float width);
  Segment(Segment&&) = default;
  Segment(const Segment&) = default;

  const Vector2d& position() const;
  const Vector2d& direction() const;
  float width() const;

  void translate(const Vector2d& delta);

 private:
  Vector2d _position;
  Vector2d _direction;
  float _width;
};
}
}