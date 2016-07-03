#pragma once

#include "Common\DeviceResources.h"
#include "Common\StepTimer.h"
#include "TreeRenderer.hpp"

// Renders Direct2D and 3D content on the screen.
namespace Tamarak {
class TamarakMain : public DX::IDeviceNotify {
 public:
  TamarakMain(const std::shared_ptr<DX::DeviceResources>& deviceResources);
  ~TamarakMain();
  void CreateWindowSizeDependentResources();
  void TrackingUpdate(float positionX) { _pointerLocationX = positionX; }
  void StartRenderLoop();
  void StopRenderLoop();
  Concurrency::critical_section& GetCriticalSection() { return _criticalSection; }

  // IDeviceNotify
  virtual void OnDeviceLost();
  virtual void OnDeviceRestored();

 private:
  void ProcessInput();
  void Update();
  bool Render();

  // Cached pointer to device resources.
  std::shared_ptr<DX::DeviceResources> _deviceResources;

  std::unique_ptr<TreeRenderer> _treeRenderer;

  Windows::Foundation::IAsyncAction ^ _renderLoopWorker;
  Concurrency::critical_section _criticalSection;

  // Rendering loop timer.
  DX::StepTimer _timer;

  // Track current input pointer position.
  float _pointerLocationX;
};
}