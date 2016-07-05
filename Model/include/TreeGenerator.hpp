#pragma once

#include <random>

namespace Tamarak {
namespace Model {

class Branch;
class Segment;
class Tree;
class Trunk;
class Vector2d;

class TreeGenerator {
 public:
  TreeGenerator();

  Tree generateTree();

 private:
  float getTrunkHeight();

  Trunk generateWobblyTrunk();
  Branch generateBranch(const Vector2d& startPos, const Vector2d& direction, float initialWidth, float finalWidth,
                        int depth, float length);
  void decorateSubBranches(Branch& target, int depth);
  float createNewAngle(const std::vector<float> usedAngles, float minAngle);
  std::vector<Branch> generateBranches(const Vector2d& position, float maxWidth, int maxDepth);
  std::vector<Segment> generateWobblyBody(const float length, const Vector2d& direction, float initialWidth,
                                          float finalWidth, bool biasTowardsTargetDirection = false);

  std::mt19937 _rand;
};
}
}