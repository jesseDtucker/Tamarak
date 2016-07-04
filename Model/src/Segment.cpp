#include "Segment.hpp"

using namespace Tamarak::Model;

Segment::Segment(const Vector2d& position, const Vector2d& direction, float width)
    : _position(position), _direction(direction.unitVector()), _width(width) {}

void Segment::translate(const Vector2d& delta) { _position += delta; }

bool Segment::operator==(const Segment& other) const {
  return _position == other._position && _direction == other._direction && _width == other.width();
}

bool Segment::operator!=(const Segment& other) const { return !(*this == other); }
