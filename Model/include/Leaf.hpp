#pragma once

#include "PropertyHelper.hpp"
#include "Vector2d.hpp"

namespace Tamarak {
  namespace Model {
    class Leaf final {
    public:
      Leaf(const Vector2d& position, const Vector2d& direction);
      Leaf(const Leaf&) = default;
      Leaf(Leaf&&) = default;

      Leaf& operator=(const Leaf&) = default;

      bool operator==(const Leaf&) const;
      bool operator!=(const Leaf&) const;

      PROP_SET_AND_GET(Vector2d, position);
      PROP_SET_AND_GET(Vector2d, direction);
    private:
    };
  }
}