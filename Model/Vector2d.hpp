#pragma once

namespace Tamarack {
  namespace Model {
    class Vector2d final {
    public:
      Vector2d();
      Vector2d(float x, float y);
      Vector2d(const Vector2d&) = default;
      Vector2d(Vector2d&&) = default;

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
    private:
      float _x;
      float _y;
    };
  }
}