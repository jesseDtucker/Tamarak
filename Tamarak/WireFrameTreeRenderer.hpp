#pragma once

#include <D2d1.h>
#include <memory>
#include <mutex>

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
  void drawSegments(const std::vector<Model::Segment>& segments, ID2D1DeviceContext2& context);
  void drawBranches(const std::vector<Model::Branch>& branches, ID2D1DeviceContext2& context);
  void drawBranch(const Model::Branch& branch, ID2D1DeviceContext2& context);
  void drawTrunk(const Model::Trunk& trunk, ID2D1DeviceContext2& context);

  Model::Tree _tree;

  std::shared_ptr<DX::DeviceResources> _deviceResources;
  ID2D1SolidColorBrush* _directionBrush;
  ID2D1SolidColorBrush* _crossSectionBrush;
  ID2D1SolidColorBrush* _outlineBrush;
};
}