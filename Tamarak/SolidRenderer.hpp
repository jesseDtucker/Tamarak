#pragma once

#include <D2d1.h>
#include <memory>
#include <mutex>
#include <vector>

#include "DeviceResources.h"
#include "RenderUtils.hpp"
#include "Tree.hpp"

typedef std::unique_ptr<ID2D1PathGeometry, comDeleter<ID2D1PathGeometry>> pathPtr;

namespace Tamarak {
  class SolidRenderer final {
  public:
    SolidRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources);

    void render();
    void update(const DX::StepTimer& timer);
    void setTree(const Model::Tree& tree);

    void notifyScreenSizeChanged();

  private:

    Model::Tree _tree;
    std::vector<pathPtr> _geometry;

    std::shared_ptr<DX::DeviceResources> _deviceResources;
    ID2D1SolidColorBrush* _brownBrush;
  };
}