#include "pch.h"

#include "Common\DirectXHelper.h"
#include "TamarakMain.h"

using namespace std;
using namespace Windows::Foundation;
using namespace Windows::System::Threading;
using namespace Concurrency;
using namespace Tamarak;
using namespace Tamarak::Model;

// Loads and initializes application assets when the application is loaded.
TamarakMain::TamarakMain(const std::shared_ptr<DX::DeviceResources>& deviceResources)
    : _deviceResources(deviceResources), _pointerLocationX(0.0f) {
  // Register to be notified if the Device is lost or recreated
  _deviceResources->RegisterDeviceNotify(this);

  // TODO: Replace this with your app's content initialization.
  _wireFrameRenderer = make_unique<WireFrameTreeRenderer>(deviceResources);
  _solidRenderer = make_unique<SolidRenderer>(deviceResources);
  Tree tree = _treeGenerator.generateTree();
  _wireFrameRenderer->setTree(tree);
  _solidRenderer->setTree(tree);

  // TODO: Change the timer settings if you want something other than the default variable timestep mode.
  // e.g. for 60 FPS fixed timestep update logic, call:
  /*
  m_timer.SetFixedTimeStep(true);
  m_timer.SetTargetElapsedSeconds(1.0 / 60);
  */
}

TamarakMain::~TamarakMain() {
  // Deregister device notification
  _deviceResources->RegisterDeviceNotify(nullptr);
}

// Updates application state when the window size changes (e.g. device orientation change)
void TamarakMain::CreateWindowSizeDependentResources() { _wireFrameRenderer->notifyScreenSizeChanged(); _solidRenderer->notifyScreenSizeChanged(); }

void TamarakMain::StartRenderLoop() {
  // If the animation render loop is already running then do not start another thread.
  if (_renderLoopWorker != nullptr && _renderLoopWorker->Status == AsyncStatus::Started) {
    return;
  }

  // Create a task that will be run on a background thread.
  auto workItemHandler = ref new WorkItemHandler([this](IAsyncAction ^ action) {
    // Calculate the updated frame and render once per vertical blanking interval.
    while (action->Status == AsyncStatus::Started) {
      critical_section::scoped_lock lock(_criticalSection);
      Update();
      if (Render()) {
        _deviceResources->Present();
      }
    }
  });

  // Run task on a dedicated high priority background thread.
  _renderLoopWorker = ThreadPool::RunAsync(workItemHandler, WorkItemPriority::High, WorkItemOptions::TimeSliced);
}

void TamarakMain::StopRenderLoop() { _renderLoopWorker->Cancel(); }

// Updates the application state once per frame.
void TamarakMain::Update() {
  ProcessInput();

  // Update scene objects.
  _timer.Tick([&]() { _wireFrameRenderer->update(_timer); _solidRenderer->update(_timer); });
}

// Process all input from the user before updating game state
void TamarakMain::ProcessInput() {
  // TODO: Add per frame input handling here.
}

// Renders the current frame according to the current application state.
// Returns true if the frame was rendered and is ready to be displayed.
bool TamarakMain::Render() {
  // Don't try to render anything before the first Update.
  if (_timer.GetFrameCount() == 0) {
    return false;
  }

  lock_guard<decltype(_syncLock)> lock(_syncLock);

  auto context = _deviceResources->GetD3DDeviceContext();

  // Reset the viewport to target the whole screen.
  auto viewport = _deviceResources->GetScreenViewport();
  context->RSSetViewports(1, &viewport);

  // Reset render targets to the screen.
  ID3D11RenderTargetView* const targets[1] = {_deviceResources->GetBackBufferRenderTargetView()};
  context->OMSetRenderTargets(1, targets, _deviceResources->GetDepthStencilView());

  // Clear the back buffer and depth stencil view.
  context->ClearRenderTargetView(_deviceResources->GetBackBufferRenderTargetView(), DirectX::Colors::CornflowerBlue);
  context->ClearDepthStencilView(_deviceResources->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f,
                                 0);

  auto context2d = _deviceResources->GetD2DDeviceContext();
  context2d->BeginDraw();
  context2d->Clear(D2D1::ColorF(0xB6BFBB));

  // _wireFrameRenderer->render();
  _solidRenderer->render();

  context2d->EndDraw();

  return true;
}

// Notifies renderers that device resources need to be released.
void TamarakMain::OnDeviceLost() {
  // TODO::JT
}

// Notifies renderers that device resources may now be recreated.
void TamarakMain::OnDeviceRestored() {
  // TODO::JT
  CreateWindowSizeDependentResources();
}

void Tamarak::TamarakMain::createNewTree() {
  lock_guard<decltype(_syncLock)> lock(_syncLock);
  Tree tree = _treeGenerator.generateTree();
  _wireFrameRenderer->setTree(tree);
  _solidRenderer->setTree(tree);
}
