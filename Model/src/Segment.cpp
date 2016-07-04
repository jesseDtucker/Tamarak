#include "Segment.hpp"

using namespace Tamarak::Model;

Segment::Segment(const Vector2d& position, const Vector2d& direction, float width)
    : _position(position), _direction(direction), _width(width) {}

const Vector2d& Segment::position() const { return _position; }

const Vector2d& Segment::direction() const { return _direction; }

float Segment::width() const { return _width; }

void Segment::translate(const Vector2d& delta) { _position += delta; }
