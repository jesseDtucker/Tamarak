#pragma once

#include "d2d1.h"

namespace Tamarak {
  namespace Model {
    class Tree;
  }
  void placeTree(Model::Tree& tree, const D2D1_SIZE_F& screenSize, bool rescaleTree = false);
}