#pragma once

namespace Tamarak {
namespace Model {
class Vector2d final {
 public:
  Vector2d();
  Vector2d(float x, float y);
  Vector2d(const Vector2d&) = default;
  Vector2d(Vector2d&&) = default;

  Vector2d& operator=(const Vector2d&) = default;

  float x() const;
  float y() const;

  float magnitude() const;
  Vector2d unitVector() const;

  Vector2d operator+(const Vector2d&) const;
  Vector2d operator-(const Vector2d&) const;
  Vector2d operator*(const float) const;
  Vector2d operator/(const float) const;

  Vector2d& operator+=(const Vector2d&);
  Vector2d& operator-=(const Vector2d&);
  Vector2d& operator*=(const float);
  Vector2d& operator/=(const float);

  bool operator==(const Vector2d&);
  bool operator!=(const Vector2d&);

 private:
  float _x;
  float _y;
};
}
}