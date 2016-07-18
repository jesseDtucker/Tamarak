#include "RenderUtils.hpp"

#include <vector>

#include "Branch.hpp"
#include "Segment.hpp"
#include "Tree.hpp"
#include "Trunk.hpp"

using namespace std;
using namespace Tamarak;
using namespace Tamarak::Model;

static const float PIXELS_PER_METER = 48.f;

static void scaleSegmentsToScreenSpace(vector<Segment>& segments) {
  for (auto& segment : segments) {
    segment.Setwidth(segment.width() * PIXELS_PER_METER);
    segment.Setposition(segment.position() * PIXELS_PER_METER * -1.0f);
    segment.Setdirection(segment.direction() * -1.0f);
  }
}

static void scaleBranchToScreenSpace(Branch& branch);
static void scaleBranchesToScreenSpace(vector<Branch>& branches) {
  for (auto& branch : branches) {
    scaleBranchToScreenSpace(branch);
  }
}

static void scaleBranchToScreenSpace(Branch& branch) {
  scaleSegmentsToScreenSpace(branch.Mutablebody());
  scaleBranchesToScreenSpace(branch.Mutablebranches());
}

static void translateSegments(vector<Segment>& segments, const Vector2d& delta) {
  for (auto& segment : segments) {
    segment.translate(delta);
  }
}

static void translateBranches(vector<Branch>& branches, const Vector2d& delta);
static void translateBranch(Branch& branch, const Vector2d& delta) {
  translateSegments(branch.Mutablebody(), delta);
  translateBranches(branch.Mutablebranches(), delta);
}

static void translateBranches(vector<Branch>& branches, const Vector2d& delta) {
  for (auto& branch : branches) {
    translateBranch(branch, delta);
  }
}

static Vector2d treePos(D2D1_SIZE_F screenSize) {
  static const Vector2d BASE_POS = { 0.5f, 0.95f };
  return Vector2d{ BASE_POS.x() * screenSize.width, BASE_POS.y() * screenSize.height };
}

void Tamarak::placeTree(Model::Tree& tree, const D2D1_SIZE_F& screenSize, bool rescaleTree)
{
  if (rescaleTree) {
    scaleSegmentsToScreenSpace(tree.Mutabletrunk().Mutablebody());
    scaleBranchesToScreenSpace(tree.Mutablebranches());
  }

  Vector2d pos = treePos(screenSize);
  Vector2d oldPos;
  if (tree.trunk().body().size() > 0) {
    oldPos = begin(tree.trunk().body())->position();
  }
  auto delta = pos - oldPos;
  translateSegments(tree.Mutabletrunk().Mutablebody(), delta);
  translateBranches(tree.Mutablebranches(), delta);
}
