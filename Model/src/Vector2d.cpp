#include "Vector2d.hpp"

#include <cmath>

using namespace Tamarak::Model;

Vector2d::Vector2d() : Vector2d(0.0f, 0.0f) {}

Vector2d::Vector2d(float x, float y) {
  _x = x;
  _y = y;
}

float Vector2d::x() const { return _x; }

float Vector2d::y() const { return _y; }

Vector2d Vector2d::operator+(const Vector2d& other) const { return {_x + other._x, _y + other._y}; }

Vector2d Vector2d::operator-(const Vector2d& other) const { return {_x - other._x, _y - other._y}; }

Vector2d Vector2d::operator*(const float val) const { return {_x * val, _y * val}; }

Vector2d Vector2d::operator/(const float val) const { return *this * (1.0f / val); }

Vector2d& Vector2d::operator+=(const Vector2d& other) {
  _x += other._x;
  _y += other._y;
  return *this;
}

Vector2d& Vector2d::operator-=(const Vector2d& other) {
  _x -= other._x;
  _y -= other._y;
  return *this;
}

Vector2d& Vector2d::operator*=(const float scale) {
  _x *= scale;
  _y *= scale;
  return *this;
}

Vector2d& Vector2d::operator/=(const float scale) {
  _x /= scale;
  _y /= scale;
  return *this;
}

float Vector2d::magnitude() const { return sqrtf(_x * _x + _y * _y); }

Vector2d Vector2d::unitVector() const { return *this / magnitude(); }

bool Tamarak::Model::Vector2d::operator==(const Vector2d& other) const { return _x == other._x && _y == other._y; }

bool Tamarak::Model::Vector2d::operator!=(const Vector2d& other) const { return !(*this == other); }

float Tamarak::Model::Vector2d::angleBetween(const Vector2d& other) const {
  // auto test = atan2(_y - other._y, _x - other._x);
  // auto test2 = atan2(_y - other._y, _x - other._x);
  // auto test3 = acos(this->dotProduct(other));
  return atan2(other._y, other._x) - atan2(_y, _x);
  // return acos(this->dotProduct(other));
  // return atan2(other._y - _y, other._x - _x);
}

float Tamarak::Model::Vector2d::dotProduct(const Vector2d& other) const { return _x * other._x + _y * other._y; }

void Vector2d::rotate(float radians) {
  float ca = cos(radians);
  float sa = sin(radians);
  auto oldX = _x;
  auto oldY = _y;
  _x = ca * oldX - sa * oldY;
  _y = sa * oldX + ca * oldY;
}
