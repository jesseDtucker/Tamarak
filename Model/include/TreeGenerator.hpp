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
  std::vector<Segment> generateWobblyBody(const float length, const Vector2d& direction, float initialWidth,
                                          float finalWidth, bool biasTowardsTargetDirection = false);

  std::mt19937 _rand;
};
}
}