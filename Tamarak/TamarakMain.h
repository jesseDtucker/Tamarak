#pragma once

#include "Common\DeviceResources.h"
#include "Common\StepTimer.h"
#include "SolidRenderer.hpp"
#include "TreeGenerator.hpp"
#include "WireFrameTreeRenderer.hpp"

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

  void createNewTree();

 private:
  void ProcessInput();
  void Update();
  bool Render();

  // Cached pointer to device resources.
  std::shared_ptr<DX::DeviceResources> _deviceResources;

  std::unique_ptr<WireFrameTreeRenderer> _wireFrameRenderer;
  std::unique_ptr<SolidRenderer> _solidRenderer;

  Windows::Foundation::IAsyncAction ^ _renderLoopWorker;
  Concurrency::critical_section _criticalSection;

  // Rendering loop timer.
  DX::StepTimer _timer;

  // Track current input pointer position.
  float _pointerLocationX;

  Model::TreeGenerator _treeGenerator;
};
}