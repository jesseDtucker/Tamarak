#pragma once

#include <memory>

#include "DeviceResources.h"

namespace Tamarak {
class TreeRenderer final {
 public:
  TreeRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources);

  void render();
  void update(const DX::StepTimer& timer);

 private:
  std::shared_ptr<DX::DeviceResources> _deviceResources;
};
}