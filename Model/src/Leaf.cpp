#include "Leaf.hpp"

using namespace Tamarak::Model;

Leaf::Leaf(const Vector2d& position, const Vector2d& direction)
  :_position(position)
  , _direction(direction)
{

}

bool Leaf::operator==(const Leaf& other) const
{
  return _position == other._position && _direction == other._direction;
}

bool Leaf::operator!=(const Leaf& other) const
{
  return !(*this == other);
}

