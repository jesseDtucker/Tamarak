#include "Vector2d.hpp"

#include <cmath>

using namespace Tamarack::Model;

Vector2d::Vector2d() : Vector2d(0.0f, 0.0f) { }

Vector2d::Vector2d(float x, float y)
{
  _x = x;
  _y = y;
}

float Vector2d::x() const
{
  return _x;
}

float Vector2d::y() const
{
  return _y;
}

Vector2d Vector2d::operator+(const Vector2d& other) const
{
  return { _x + other._x, _y + other._y };
}

Vector2d Vector2d::operator-(const Vector2d& other) const
{
  return { _x - other._x, _y - other._y };
}

Vector2d Vector2d::operator*(const float val) const
{
  return { _x * val, _y * val };
}

Vector2d Vector2d::operator/(const float val) const
{
  return *this * (1.0f / val);
}

Vector2d& Vector2d::operator+=(const Vector2d& other)
{
  _x += other._x;
  _y += other._y;
  return *this;
}

Vector2d& Vector2d::operator-=(const Vector2d& other)
{
  _x -= other._x;
  _y -= other._y;
  return *this;
}

Vector2d& Vector2d::operator*=(const float scale)
{
  _x *= scale;
  _y *= scale;
  return *this;
}

Vector2d& Vector2d::operator/=(const float scale)
{
  return *this * (1.0f/ scale);
}

float Vector2d::magnitude() const
{
  return sqrtf(_x * _x + _y * _y);
}

Vector2d Vector2d::unitVector() const
{
  return *this / magnitude();
}
