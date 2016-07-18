#include "TreeGenerator.hpp"

#include <algorithm>
#include <cmath>
#include <vector>

#include "Tree.hpp"

using namespace std;
using namespace Tamarak::Model;

const float SEGMENT_SIZE = 0.1f;
static const float PI = 3.14159f;

Tamarak::Model::TreeGenerator::TreeGenerator() : _rand(random_device()()) {}

static void translateSegments(vector<Segment>& segments, const Vector2d& delta) {
  for (auto& segment : segments) {
    segment.translate(delta);
  }
}

Tamarak::Model::Branch Tamarak::Model::TreeGenerator::generateBranch(const Vector2d& startPos,
                                                                     const Vector2d& direction, float initialWidth,
                                                                     float finalWidth, int depth, float length) {
  if (depth <= 0) {
    finalWidth = 0.f;
  }

  auto depthDistribution = uniform_int_distribution<int>(1, 3);

  // Make the main body
  auto body = generateWobblyBody(length, direction, initialWidth, finalWidth);
  translateSegments(body, startPos);
  Branch branch{body};

  // now add some branches along the body
  auto subBranchDepth = depth - depthDistribution(_rand);
  subBranchDepth = max(subBranchDepth, 0);
  decorateSubBranches(branch, subBranchDepth);

  // now add some branches at the end of the body
  if (depth > 0) {
    auto branchEnd = *rbegin(branch.body());
    int nextDepth = depth - depthDistribution(_rand);
    nextDepth = max(nextDepth, 0);
    auto branches = generateBranches(branchEnd.position(), branchEnd.width(), nextDepth);
    branch.Setbranches(branches);
  }

  return branch;
}

void Tamarak::Model::TreeGenerator::decorateSubBranches(Branch& target, int depth) {
  if (target.body().size() == 0) {
    return;
  }

  const float MIN_ANGLE = 20.f * 2.f * PI / 360.f;
  const float MAX_ANGLE = 60.f * 2.f * PI / 360.f;

  auto angles = {-MAX_ANGLE, -MIN_ANGLE, MIN_ANGLE, MAX_ANGLE};
  auto wieghts = {1.0, 0.0, 1.0};
  auto angleDistribution = piecewise_constant_distribution<float>(begin(angles), end(angles), begin(wieghts));
  auto depthDistribution = uniform_int_distribution<int>(1, 3);
  auto widthDistribution = uniform_real_distribution<float>(0.3f, 0.7f);
  auto lenghtDistribution = uniform_real_distribution<float>(0.3f, 0.6f);
  auto midBranchDistribution = uniform_real_distribution<float>(0.5f, 1.5f);

  // step along the body adding subbranches until the end is reached.
  float distanceToNextSubBranch = midBranchDistribution(_rand);
  auto firstSegment = *begin(target.body());
  auto lastSegment = *rbegin(target.body());
  float branchLength = (lastSegment.position() - firstSegment.position()).magnitude();
  for (auto& segment : target.body()) {
    auto distanceTraveled = (segment.position() - firstSegment.position()).magnitude();
    if (distanceTraveled > distanceToNextSubBranch) {
      auto angle = angleDistribution(_rand);
      auto direction = segment.direction();
      direction.rotate(angle);
      auto length = lenghtDistribution(_rand) * branchLength;
      auto pos = segment.position();
      auto startWidth = widthDistribution(_rand) * segment.width();
      auto endWidth = widthDistribution(_rand) * startWidth;
      int nextDepth = depth - depthDistribution(_rand);
      nextDepth = max(nextDepth, 0);
      Branch subBranch = generateBranch(pos, direction, startWidth, endWidth, nextDepth, length);
      target.Mutablebranches().push_back(subBranch);
    }
  }
}

float TreeGenerator::createNewAngle(const vector<float> usedAngles, float minAngle) {
  static const float ANGLE_RANGE = 60.f * 2.f * PI / 360.f;
  auto distribution = uniform_real_distribution<float>(-ANGLE_RANGE, ANGLE_RANGE);
  float angle = 0.0f;
  bool hasAngle = false;
  while (!hasAngle) {
    angle = distribution(_rand);
    hasAngle = !any_of(begin(usedAngles), end(usedAngles), [angle, minAngle](const auto& usedAngle) {
      auto upperBound = usedAngle + minAngle;
      auto lowerBound = usedAngle - minAngle;
      return angle < upperBound && angle > lowerBound;
    });
  }

  return angle;
}

vector<Branch> TreeGenerator::generateBranches(const Vector2d& position, float maxWidth, int maxDepth) {
  const float MIN_ANGLE = 15 * 2.f * PI / 360.f;
  int numBranches = uniform_int_distribution<int>(2, 5)(_rand);
  int minDepth = 2;
  if (minDepth > maxDepth) {
    minDepth = 0;
  }
  auto depthDistribution = uniform_int_distribution<int>(minDepth, maxDepth);
  auto lengthDistribution = uniform_real_distribution<float>(1.f, 3.f);
  maxWidth = max(maxWidth, 0.2f);
  float minWidth = maxWidth * 0.3f;
  auto widthDistribution = uniform_real_distribution<float>(minWidth, maxWidth);
  vector<float> usedAngles;
  vector<Branch> branches;

  while (branches.size() < numBranches) {
    // first figure out which way the branch will go
    float branchAngle = createNewAngle(usedAngles, MIN_ANGLE);
    usedAngles.push_back(branchAngle);
    auto direction = Vector2d{0.0f, 1.0f};
    direction.rotate(branchAngle);

    // how deep does it go?
    int depth = depthDistribution(_rand);

    // how wide is the branch?
    float startWidth = widthDistribution(_rand);
    float endWidth = widthDistribution(_rand);
    if (endWidth > startWidth) {
      swap(endWidth, startWidth);
    }

    // how long is it?
    auto length = lengthDistribution(_rand);

    branches.push_back(generateBranch(position, direction, startWidth, endWidth, depth, length));
  }

  return branches;
}

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

Tree TreeGenerator::generateTree() {
  auto trunk = generateWobblyTrunk();
  const auto& trunkEnd = *rbegin(trunk.body());
  auto branches = generateBranches(trunkEnd.position(), trunkEnd.width(), 2);
  return {trunk, branches};
}
