#include "TreeGenerator.hpp"

#include <cmath>
#include <vector>

#include "Tree.hpp"

using namespace std;
using namespace Tamarak::Model;

const float SEGMENT_SIZE = 0.1f;
static const float PI = 3.14159f;

Tamarak::Model::TreeGenerator::TreeGenerator() : _rand(random_device()()) {}

std::vector<Segment> Tamarak::Model::TreeGenerator::generateWobblyBody(const float length,
                                                                       const Vector2d& targetDirection,
                                                                       float initialWidth, float finalWidth,
                                                                       bool biasTowardsTargetDirection) {
  static const float MAX_ANGLE = 15.0f / 360.f * 2.f * PI;
  static const float MAX_DELTA_ANGLE = 3.0f / 360.f * 2.f * PI;

  uniform_real_distribution<float> randAngle(-MAX_DELTA_ANGLE, MAX_DELTA_ANGLE);

  auto direction = targetDirection.unitVector();
  Vector2d lastPoint = {0.0f, 0.0f};
  Vector2d lastDirection = direction;

  std::vector<Segment> body;

  while (lastPoint.magnitude() < length) {
    // First pick a direction for the next segment
    Vector2d nextDirection = lastDirection;
    nextDirection.rotate(randAngle(_rand));
    auto angleFromDirection = direction.angleBetween(nextDirection);
    if (abs(angleFromDirection) > MAX_ANGLE) {
      nextDirection = direction;
      if (angleFromDirection > 0.0f) {
        nextDirection.rotate(MAX_ANGLE);
      } else {
        nextDirection.rotate(-1.f * MAX_ANGLE);
      }
    }

    auto fractionCreated = lastPoint.magnitude() / length;
    static const float BIAS_THRESHOLD = 0.75f;
    if (biasTowardsTargetDirection && fractionCreated > BIAS_THRESHOLD) {
      float bias = (1.0f - fractionCreated) / (1.f - BIAS_THRESHOLD);
      nextDirection = direction;
      nextDirection.rotate(bias * angleFromDirection);
    }

    // Next determine the width at this point. Assume linear drop off from start to finish
    auto width = (1.0f - fractionCreated) * (initialWidth - finalWidth) + finalWidth;
    // Finally the point this segment is located at.
    auto pos = lastPoint + nextDirection * SEGMENT_SIZE;

    body.push_back({pos, nextDirection, width});

    lastPoint = pos;
    lastDirection = nextDirection;
  }

  return body;
}

static Trunk generateBasicTrunk() {
  vector<Segment> trunkBody;
  for (int i = 0; i < 5; ++i) {
    trunkBody.push_back({{0.0f, float(i)}, {0.0f, 1.0f}, 0.5f});
  }
  return {trunkBody};
}

float TreeGenerator::getTrunkHeight() { return uniform_real_distribution<float>(3.f, 6.f)(_rand); }

Trunk TreeGenerator::generateWobblyTrunk() {
  auto height = getTrunkHeight();
  Vector2d up = {0.0f, 1.0f};
  uniform_real_distribution<float> randWidth(0.2f, 0.4f);
  auto startWidth = randWidth(_rand);
  auto endWidth = randWidth(_rand);

  if (endWidth > startWidth) {
    swap(startWidth, endWidth);
  }

  return {generateWobblyBody(height, up, startWidth, endWidth, true)};
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
  auto trunk = generateWobblyTrunk();
  return {trunk, generateBranches(trunk)};
}
