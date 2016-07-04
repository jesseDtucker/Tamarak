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

  void notifyScreenSizeChanged();

 private:
  void drawSegments(ID2D1DeviceContext2& context);

  Model::Tree _tree;
  std::vector<Model::Segment> _segments;
  D2D1_SIZE_F _screenSize;

  std::shared_ptr<DX::DeviceResources> _deviceResources;
  ID2D1SolidColorBrush* _directionBrush;
  ID2D1SolidColorBrush* _crossSectionBrush;
  ID2D1SolidColorBrush* _outlineBrush;
};
}