#include "pch.h"

#include "Arc_Assert.hpp"
#include "WireFrameTreeRenderer.hpp"

using namespace std;
using namespace Tamarak;

WireFrameTreeRenderer::WireFrameTreeRenderer(const shared_ptr<DX::DeviceResources>& deviceResources)
    : _deviceResources(deviceResources)
{
  auto hr = deviceResources->GetD2DDeviceContext()->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::BlueViolet), &_brush);
  ARC_ASSERT(SUCCEEDED(hr));
}

void WireFrameTreeRenderer::render() {
  auto context = _deviceResources->GetD2DDeviceContext();
  context->BeginDraw();
  context->Clear(D2D1::ColorF(D2D1::ColorF::White));

  D2D1_ELLIPSE ellipse = D2D1::Ellipse({ 100.0f, 100.0f }, 75.0f, 150.0f);

  context->FillEllipse(ellipse, _brush);

  context->EndDraw();
}

void WireFrameTreeRenderer::update(const DX::StepTimer& timer) {
  // TODO::JT
}

void WireFrameTreeRenderer::setTree(const Model::Tree& tree)
{
  _tree = tree;
}
