#include "pch.h"

#include "WireFrameTreeRenderer.hpp"

#include "Arc_Assert.hpp"

using namespace std;
using namespace Tamarak;
using namespace Tamarak::Model;

static const float PIXELS_PER_METER = 64.f;
static const float DIRECTION_LENGTH = 16.f;

static vector<Segment> segmentsFromBranch(const Branch& branch) {
  vector<Segment> segments = branch.body();
  for (const auto& childBranch : branch.branches()) {
    auto branchSegments = segmentsFromBranch(childBranch);
    move(begin(branchSegments), end(branchSegments), back_inserter(segments));
  }
  return segments;
}

static vector<Segment> segmentsFromTree(const Tree& tree) {
  vector<Segment> segments = tree.trunk().body();
  for (const auto& childBranch : tree.branches()) {
    auto branchSegments = segmentsFromBranch(childBranch);
    move(begin(branchSegments), end(branchSegments), back_inserter(segments));
  }
  return segments;
}

static void scaleSegmentsToScreenSpace(vector<Segment>& segments) {
  for (auto& segment : segments) {
    segment.Setwidth(segment.width() * PIXELS_PER_METER);
    segment.Setposition(segment.position() * PIXELS_PER_METER * -1.0f);
    segment.Setdirection(segment.direction() * -1.0f);
  }
}

static void translateSegments(vector<Segment>& segments, const Vector2d& delta) {
  for (auto& segment : segments) {
    segment.translate(delta);
  }
}

static void drawVector(const Vector2d& start, const Vector2d& end, ID2D1DeviceContext2& context, ID2D1Brush& brush) {
  context.DrawLine({start.x(), start.y()}, {end.x(), end.y()}, &brush);
}

WireFrameTreeRenderer::WireFrameTreeRenderer(const shared_ptr<DX::DeviceResources>& deviceResources)
    : _deviceResources(deviceResources) {
  auto hr = deviceResources->GetD2DDeviceContext()->CreateSolidColorBrush(D2D1::ColorF(0xDB3F1B), &_directionBrush);
  ARC_ASSERT(SUCCEEDED(hr));
  hr = deviceResources->GetD2DDeviceContext()->CreateSolidColorBrush(D2D1::ColorF(0x4FC6DB), &_crossSectionBrush);
  ARC_ASSERT(SUCCEEDED(hr));
  hr = deviceResources->GetD2DDeviceContext()->CreateSolidColorBrush(D2D1::ColorF(0xDFE9EB), &_outlineBrush);
  ARC_ASSERT(SUCCEEDED(hr));
}

void WireFrameTreeRenderer::render() {
  auto context = _deviceResources->GetD2DDeviceContext();
  context->BeginDraw();
  context->Clear(D2D1::ColorF(0x38393B));

  drawSegments(*context);

  context->EndDraw();
}

void WireFrameTreeRenderer::drawSegments(ID2D1DeviceContext2& context) {
  auto width = context.GetSize().width;
  auto height = context.GetSize().height;

  Vector2d lastCrossSectionPoint1;
  Vector2d lastCrossSectionPoint2;
  bool hasDrawnFirst = false;
  for (const auto& segment : _segments) {
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

void WireFrameTreeRenderer::update(const DX::StepTimer& timer) {
  // TODO::JT
}

void WireFrameTreeRenderer::setTree(const Tree& tree) {
  _tree = tree;
  _segments = segmentsFromTree(_tree);
  scaleSegmentsToScreenSpace(_segments);
}

void Tamarak::WireFrameTreeRenderer::notifyScreenSizeChanged() {
  auto context = _deviceResources->GetD2DDeviceContext();
  auto newScreenSize = context->GetSize();
  translateSegments(_segments, Vector2d{_screenSize.width / 2.f, _screenSize.height / 3.f * 2.f} * -1.0f);
  translateSegments(_segments, {newScreenSize.width / 2.f, newScreenSize.height / 3.f * 2.f});
  _screenSize = newScreenSize;
}
