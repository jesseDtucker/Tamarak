#include "pch.h"

#include <algorithm>
#include <memory>

#include "Arc_Assert.hpp"
#include "Segment.hpp"
#include "SolidRenderer.hpp"
#include "RenderUtils.hpp"

using namespace std;
using namespace Tamarak;
using namespace Tamarak::Model;

static vector<D2D1_POINT_2F> getPoints(const vector<Segment>& body) {
  vector<D2D1_POINT_2F> points;
  points.reserve(body.size() * 2);

  // get the left side first
  transform(begin(body), end(body), back_inserter(points), [](const auto& segment) {
    Vector2d normalToDirection{ -1 * segment.direction().y(), segment.direction().x() };
    Vector2d point = segment.position() + normalToDirection * segment.width() / 2;
    return D2D1_POINT_2F{ point.x(), point.y() };
  });

  // now the right side
  transform(rbegin(body), rend(body), back_inserter(points), [](const auto& segment) {
    Vector2d normalToDirection{ -1 * segment.direction().y(), segment.direction().x() };
    Vector2d point = segment.position() + normalToDirection * segment.width() / 2 * -1;
    return D2D1_POINT_2F{ point.x(), point.y() };
  });

  return points;
}

static unique_ptr<ID2D1PathGeometry> createGeometry(const vector<Segment>& body, ID2D1Factory* factory) {
  ARC_ASSERT(body.size() > 0);
  ARC_ASSERT(factory != nullptr);
  ID2D1PathGeometry* geometry = nullptr;
  auto hr = factory->CreatePathGeometry(&geometry);
  ARC_ASSERT(SUCCEEDED(hr));
  ID2D1GeometrySink* sink = nullptr;
  hr = geometry->Open(&sink);
  ARC_ASSERT(SUCCEEDED(hr));

  auto points = getPoints(body);
  sink->BeginFigure(*begin(points), D2D1_FIGURE_BEGIN_FILLED);
  sink->AddLines(points.data() + 1, points.size() - 1);
  sink->EndFigure(D2D1_FIGURE_END_CLOSED);
  hr = sink->Close();
  ARC_ASSERT(SUCCEEDED(hr));

  return unique_ptr<ID2D1PathGeometry>{ geometry };
}

static void createGeometry(const vector<Branch> branches, ID2D1Factory *factory, vector<unique_ptr<ID2D1PathGeometry>>& geometry) {
  for (const auto& branch : branches) {
    geometry.push_back(createGeometry(branch.body(), factory));
    createGeometry(branch.branches(), factory, geometry);
  }
}

static vector<unique_ptr<ID2D1PathGeometry>> createGeometry(const Tree& tree, ID2D1Factory *factory) {
  vector<unique_ptr<ID2D1PathGeometry>> geometry;

  geometry.push_back(createGeometry(tree.trunk().body(), factory));
  createGeometry(tree.branches(), factory, geometry);

  return geometry;
}

SolidRenderer::SolidRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources)
  : _deviceResources(deviceResources)
{
  auto hr = deviceResources->GetD2DDeviceContext()->CreateSolidColorBrush(D2D1::ColorF(0xB27D67), &_brownBrush);
  ARC_ASSERT(SUCCEEDED(hr));
}

void SolidRenderer::render()
{
  auto context = _deviceResources->GetD2DDeviceContext();

  for (const auto& geo : _geometry) {
    context->FillGeometry(geo.get(), _brownBrush);
  }
}

void SolidRenderer::update(const DX::StepTimer& timer)
{

}

void SolidRenderer::setTree(const Model::Tree& tree)
{
  lock_guard<decltype(_syncLock)> lock(_syncLock);
  _tree = tree;
  auto context = _deviceResources->GetD2DDeviceContext();
  auto screenSize = context->GetSize();

  placeTree(_tree, screenSize, true);
  //_geometry = createGeometry(tree, _deviceResources->GetD2DFactory());
}

void SolidRenderer::notifyScreenSizeChanged()
{
  lock_guard<decltype(_syncLock)> lock(_syncLock);
  auto context = _deviceResources->GetD2DDeviceContext();
  auto screenSize = context->GetSize();

  placeTree(_tree, screenSize);
  if (_geometry.size() == 0) {
    _geometry = createGeometry(_tree, _deviceResources->GetD2DFactory());
  }

}

