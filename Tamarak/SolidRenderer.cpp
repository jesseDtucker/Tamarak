#include "pch.h"

#include <algorithm>
#include <memory>
#include <random>

#include "Arc_Assert.hpp"
#include "Segment.hpp"
#include "SolidRenderer.hpp"
#include "RenderUtils.hpp"

using namespace std;
using namespace Tamarak;
using namespace Tamarak::Model;

static const float PI = 3.14159f;

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

static pathPtr createGeometry(const vector<Segment>& body, ID2D1Factory* factory) {
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

  SafeRelease(&sink);

  return pathPtr{ geometry };
}

static void createGeometry(const vector<Branch> branches, ID2D1Factory *factory, vector<pathPtr>& geometry) {
  for (const auto& branch : branches) {
    geometry.push_back(createGeometry(branch.body(), factory));
    createGeometry(branch.branches(), factory, geometry);
  }
}

static vector<pathPtr> createGeometry(const Tree& tree, ID2D1Factory *factory) {
  vector<pathPtr> geometry;

  geometry.push_back(createGeometry(tree.trunk().body(), factory));
  createGeometry(tree.branches(), factory, geometry);

  return geometry;
}

static void createLeaves(const vector<Branch>& branches, vector<LeafEllipse>& leaves) {
  for (const auto& branch : branches) {
    transform(begin(branch.leaves()), end(branch.leaves()), back_inserter(leaves), [](const Leaf& leaf) {
      const float LEAF_SIZE = 8.f;
      D2D1_ELLIPSE ellipse;
      auto pos = leaf.position() + leaf.direction() * LEAF_SIZE;
      ellipse.point = { pos.x(), pos.y() };
      ellipse.radiusX = LEAF_SIZE/2.f;
      ellipse.radiusY = LEAF_SIZE;
      return LeafEllipse{ leaf.direction(), ellipse };
    });
    createLeaves(branch.branches(), leaves);
  }
}

static vector<LeafEllipse> createLeaves(const Tree& tree) {
  vector<LeafEllipse> leaves;
  createLeaves(tree.branches(), leaves);
  return leaves;
}

SolidRenderer::SolidRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources)
  : _deviceResources(deviceResources)
  , _rand(random_device()())
{
  ID2D1SolidColorBrush* brownBrush;
  ID2D1SolidColorBrush* lb1;
  ID2D1SolidColorBrush* lb2;
  ID2D1SolidColorBrush* lb3;
  ID2D1SolidColorBrush* lb4;
  ID2D1SolidColorBrush* lb5;
  auto hr = deviceResources->GetD2DDeviceContext()->CreateSolidColorBrush(D2D1::ColorF(0x593330), &brownBrush);
  ARC_ASSERT(SUCCEEDED(hr));
  hr = deviceResources->GetD2DDeviceContext()->CreateSolidColorBrush(D2D1::ColorF(0x083B02), &lb1);
  ARC_ASSERT(SUCCEEDED(hr));
  hr = deviceResources->GetD2DDeviceContext()->CreateSolidColorBrush(D2D1::ColorF(0x055432), &lb2);
  ARC_ASSERT(SUCCEEDED(hr));
  hr = deviceResources->GetD2DDeviceContext()->CreateSolidColorBrush(D2D1::ColorF(0x00541C), &lb3);
  ARC_ASSERT(SUCCEEDED(hr));
  hr = deviceResources->GetD2DDeviceContext()->CreateSolidColorBrush(D2D1::ColorF(0x005909), &lb4);
  ARC_ASSERT(SUCCEEDED(hr));
  hr = deviceResources->GetD2DDeviceContext()->CreateSolidColorBrush(D2D1::ColorF(0x205407), &lb5);
  ARC_ASSERT(SUCCEEDED(hr));

  _brownBrush = SolidColorBrushPtr{ brownBrush };
  _leafBrushOne = SolidColorBrushPtr{ lb1 };
  _leafBrushTwo = SolidColorBrushPtr{ lb2 };
  _leafBrushThree = SolidColorBrushPtr{ lb3 };
  _leafBrushFour = SolidColorBrushPtr{ lb4 };
  _leafBrushFive = SolidColorBrushPtr{ lb5 };
}

void SolidRenderer::render()
{
  auto context = _deviceResources->GetD2DDeviceContext();

  for (const auto& geo : _geometry) {
    context->FillGeometry(geo.get(), _brownBrush.get());
  }
  static const Vector2d UP{ 0.f, 1.f };
  vector<ID2D1SolidColorBrush*> leafBrushes { _leafBrushOne.get(), _leafBrushTwo.get(), _leafBrushThree.get(), _leafBrushFour.get(), _leafBrushFive.get() };
  int nextBrush = 0;
  for (auto& leaf : _leaves) {
    auto angle = leaf.direction.angleBetween(UP);
    auto scale = D2D1::Matrix3x2F::Scale(1.2f, 1.2f, leaf.ellipse.point);
    auto angleDegrees = angle * 360.f / 2.f / PI;
    auto rot = D2D1::Matrix3x2F::Rotation(angleDegrees, leaf.ellipse.point);
    context->SetTransform(rot * scale);
    context->FillEllipse(leaf.ellipse, leafBrushes[nextBrush++ % leafBrushes.size()]);
    context->SetTransform(rot);
    context->FillEllipse(leaf.ellipse, leafBrushes[nextBrush++ % leafBrushes.size()]);
    if (nextBrush > leafBrushes.size() - 1) {
      nextBrush = 0;
    }
  }
  context->SetTransform(D2D1::Matrix3x2F::Identity());
}

void SolidRenderer::update(const DX::StepTimer& timer)
{

}

void SolidRenderer::setTree(const Model::Tree& tree)
{
  _tree = tree;
  auto context = _deviceResources->GetD2DDeviceContext();
  auto screenSize = context->GetSize();

  placeTree(_tree, screenSize, true);
  _geometry = createGeometry(_tree, _deviceResources->GetD2DFactory());
  _leaves = createLeaves(_tree);
}

void SolidRenderer::notifyScreenSizeChanged()
{
  auto context = _deviceResources->GetD2DDeviceContext();
  auto screenSize = context->GetSize();

  placeTree(_tree, screenSize);
  _geometry = createGeometry(_tree, _deviceResources->GetD2DFactory());
  _leaves = createLeaves(_tree);
}

Tamarak::LeafEllipse::LeafEllipse(Vector2d dir, D2D1_ELLIPSE ell)
  : direction(dir)
  , ellipse(ell)
{ }
