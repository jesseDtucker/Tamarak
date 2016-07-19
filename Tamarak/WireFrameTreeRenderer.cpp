#include "pch.h"

#include "WireFrameTreeRenderer.hpp"

#include "Arc_Assert.hpp"
#include "RenderUtils.hpp"

using namespace std;
using namespace Tamarak;
using namespace Tamarak::Model;

static const float DIRECTION_LENGTH = 8.f;
static const float LEAF_LENGTH = 8.f;

static vector<Segment> segmentsFromBranch(const Branch& branch) {
  vector<Segment> segments = branch.body();
  for (const auto& childBranch : branch.branches()) {
    auto branchSegments = segmentsFromBranch(childBranch);
    move(begin(branchSegments), end(branchSegments), back_inserter(segments));
  }
  return segments;
}

static void drawVector(const Vector2d& start, const Vector2d& end, ID2D1DeviceContext2& context, ID2D1Brush& brush) {
  context.DrawLine({start.x(), start.y()}, {end.x(), end.y()}, &brush);
}

WireFrameTreeRenderer::WireFrameTreeRenderer(const shared_ptr<DX::DeviceResources>& deviceResources)
    : _deviceResources(deviceResources) {
  ID2D1SolidColorBrush* dirBrush;
  ID2D1SolidColorBrush* crossBrush;
  ID2D1SolidColorBrush* outlineBrush;
  ID2D1SolidColorBrush* leafBrush;
  auto hr = deviceResources->GetD2DDeviceContext()->CreateSolidColorBrush(D2D1::ColorF(0xDB3F1B), &dirBrush);
  ARC_ASSERT(SUCCEEDED(hr));
  hr = deviceResources->GetD2DDeviceContext()->CreateSolidColorBrush(D2D1::ColorF(0x4FC6DB), &crossBrush);
  ARC_ASSERT(SUCCEEDED(hr));
  hr = deviceResources->GetD2DDeviceContext()->CreateSolidColorBrush(D2D1::ColorF(0xDFE9EB), &outlineBrush);
  ARC_ASSERT(SUCCEEDED(hr));
  hr = deviceResources->GetD2DDeviceContext()->CreateSolidColorBrush(D2D1::ColorF(0x195E19), &leafBrush);
  ARC_ASSERT(SUCCEEDED(hr));
  _directionBrush = SolidColorBrushPtr{ dirBrush };
  _crossSectionBrush = SolidColorBrushPtr{ crossBrush };
  _outlineBrush = SolidColorBrushPtr{ outlineBrush };
  _leafBrush = SolidColorBrushPtr{ leafBrush };
}

void WireFrameTreeRenderer::render() {
  auto context = _deviceResources->GetD2DDeviceContext();
  drawTrunk(_tree.trunk(), *context);
  drawBranches(_tree.branches(), *context);
}

void WireFrameTreeRenderer::drawSegments(const vector<Segment>& segments, ID2D1DeviceContext2& context) {
  Vector2d lastCrossSectionPoint1;
  Vector2d lastCrossSectionPoint2;
  bool hasDrawnFirst = false;
  for (const auto& segment : segments) {
    // draw the direction vector.
    drawVector(segment.position(), segment.position() + segment.direction() * DIRECTION_LENGTH, context,
               *_directionBrush);

    // now draw the cross section vector
    Vector2d normalToDirection = {-1 * segment.direction().y(), segment.direction().x()};
    auto crossSectionStart = segment.position() + normalToDirection * segment.width() / 2;
    auto crossSectionEnd = segment.position() + normalToDirection * segment.width() / 2 * -1;
    drawVector(crossSectionStart, crossSectionEnd, context, *_crossSectionBrush);

    if (hasDrawnFirst) {
      // finally draw the outline
      drawVector(lastCrossSectionPoint1, crossSectionStart, context, *_outlineBrush);
      drawVector(lastCrossSectionPoint2, crossSectionEnd, context, *_outlineBrush);
    }
    lastCrossSectionPoint1 = crossSectionStart;
    lastCrossSectionPoint2 = crossSectionEnd;
    hasDrawnFirst = true;
  }
}

void WireFrameTreeRenderer::drawBranches(const vector<Branch>& branches, ID2D1DeviceContext2& context) {
  for (auto& branch : branches) {
    drawBranch(branch, context);
  }
}

void WireFrameTreeRenderer::drawBranch(const Branch& branch, ID2D1DeviceContext2& context) {
  drawSegments(branch.body(), context);
  drawBranches(branch.branches(), context);
  drawLeaves(branch.leaves(), context);
}

void WireFrameTreeRenderer::drawLeaves(const vector<Leaf>& leaves, ID2D1DeviceContext2& context) {
  for (const auto& leaf : leaves) {
    auto endPoint = leaf.position() + leaf.direction() * LEAF_LENGTH;
    drawVector(leaf.position(), endPoint, context, *_leafBrush);
  }
}

void WireFrameTreeRenderer::drawTrunk(const Trunk& trunk, ID2D1DeviceContext2& context) {
  drawSegments(trunk.body(), context);
}

void WireFrameTreeRenderer::update(const DX::StepTimer& timer) {
  // TODO::JT
}

static Vector2d treePos(D2D1_SIZE_F screenSize) {
  static const Vector2d BASE_POS = {0.5f, 0.95f};
  return Vector2d{BASE_POS.x() * screenSize.width, BASE_POS.y() * screenSize.height};
}

void WireFrameTreeRenderer::setTree(const Tree& tree) {
  _tree = tree;

  auto context = _deviceResources->GetD2DDeviceContext();
  auto screenSize = context->GetSize();

  placeTree(_tree, screenSize, true);
  int i = 0;
}

void WireFrameTreeRenderer::notifyScreenSizeChanged() {
  auto context = _deviceResources->GetD2DDeviceContext();
  auto screenSize = context->GetSize();

  placeTree(_tree, screenSize);
}