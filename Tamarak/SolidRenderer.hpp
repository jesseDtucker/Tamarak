#pragma once

#include <D2d1.h>
#include <memory>
#include <mutex>
#include <random>
#include <vector>

#include "DeviceResources.h"
#include "RenderUtils.hpp"
#include "Tree.hpp"

typedef std::unique_ptr<ID2D1PathGeometry, comDeleter<ID2D1PathGeometry>> pathPtr;

namespace Tamarak {

  struct LeafEllipse {
    LeafEllipse(Model::Vector2d dir, D2D1_ELLIPSE ellipse);
    Model::Vector2d direction;
    D2D1_ELLIPSE ellipse;
  };

  class SolidRenderer final {
  public:
    SolidRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources);

    void render();
    void update(const DX::StepTimer& timer);
    void setTree(const Model::Tree& tree);

    void notifyScreenSizeChanged();

  private:

    std::mt19937 _rand;
    Model::Tree _tree;
    std::vector<pathPtr> _geometry;
    std::vector<LeafEllipse> _leaves;

    std::shared_ptr<DX::DeviceResources> _deviceResources;
    SolidColorBrushPtr _brownBrush;
    SolidColorBrushPtr _leafBrushOne;
    SolidColorBrushPtr _leafBrushTwo;
    SolidColorBrushPtr _leafBrushThree;
    SolidColorBrushPtr _leafBrushFour;
    SolidColorBrushPtr _leafBrushFive;
  };
}