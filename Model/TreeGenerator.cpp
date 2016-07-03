#include "TreeGenerator.hpp"

#include <vector>

#include "Tree.hpp"

using namespace std;
using namespace Tamarack::Model;

static Trunk generateTrunk() {
  vector<Segment> trunkBody;
  for (int i = 0; i < 5; ++i) {
    trunkBody.push_back({{0.0f, float(i)}, {0.0f, 1.0f}, 0.5f});
  }
  return {trunkBody};
}

static Branch straightBranch(int length, const Vector2d& direction) {
  Vector2d unitDirection = direction.unitVector();
  vector<Segment> body;
  for (auto i = 0; i < length; ++i) {
    Vector2d point = unitDirection * float(i);
    body.push_back({point, unitDirection, 0.3f});
  }
  return {body};
}

static vector<Branch> generateBranches(const Trunk& trunk) {
  auto basePoint = trunk.body().rbegin()->position();
  vector<Branch> branches{straightBranch(5, {-0.5f, 0.5f}), straightBranch(3, {0.5f, 0.5f})};
  for (auto& branch : branches) {
    branch.translate(basePoint);
  }
  return branches;
}

Tree TreeGenerator::generateTree() {
  auto trunk = generateTrunk();
  return {trunk, generateBranches(trunk)};
}
