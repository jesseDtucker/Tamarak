#pragma once

#include <D2d1.h>
#include <memory>

#include "DeviceResources.h"
#include "Tree.hpp"

namespace Tamarak {
class WireFrameTreeRenderer final {
 public:
   WireFrameTreeRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources);

  void render();
  void update(const DX::StepTimer& timer);
  void setTree(const Model::Tree& tree);

 private:
   Model::Tree _tree;

  std::shared_ptr<DX::DeviceResources> _deviceResources;
  ID2D1SolidColorBrush *_brush;
};
}