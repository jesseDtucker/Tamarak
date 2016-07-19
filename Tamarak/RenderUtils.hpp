#pragma once

#include <memory>

#include "d2d1.h"

namespace Tamarak {
  namespace Model {
    class Tree;
  }
  void placeTree(Model::Tree& tree, const D2D1_SIZE_F& screenSize, bool rescaleTree = false);
}

template<class Interface>
inline void SafeRelease(Interface **ppInterfaceToRelease)
{
  if (*ppInterfaceToRelease != NULL)
  {
    (*ppInterfaceToRelease)->Release();

    (*ppInterfaceToRelease) = NULL;
  }
}

template<typename T>
struct comDeleter {
  VOID operator()(T* ptr) {
    ptr->Release();
  }
};

typedef std::unique_ptr<ID2D1SolidColorBrush, comDeleter<ID2D1SolidColorBrush>> SolidColorBrushPtr;