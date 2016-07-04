#include "Segment.hpp"

using namespace Tamarak::Model;

Segment::Segment(const Vector2d& position, const Vector2d& direction, float width)
    : _position(position), _direction(direction.unitVector()), _width(width) {}

void Segment::translate(const Vector2d& delta) { _position += delta; }
