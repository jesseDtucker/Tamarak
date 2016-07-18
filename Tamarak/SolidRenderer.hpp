#pragma once

#include <D2d1.h>
#include <memory>
#include <mutex>
#include <vector>

#include "DeviceResources.h"
#include "Tree.hpp"

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
    std::vector<std::unique_ptr<ID2D1PathGeometry>> _geometry;
    std::mutex _syncLock;

    std::shared_ptr<DX::DeviceResources> _deviceResources;
    ID2D1SolidColorBrush* _brownBrush;
  };
}