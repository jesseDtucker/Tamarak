#include "Branch.hpp"

using namespace std;
using namespace Tamarak::Model;

Branch::Branch(const vector<Segment>& body) : Branch(body, {}) {}

Branch::Branch(const vector<Segment>& body, const vector<Branch>& branches) : _body(body), _branches(branches) {}

void Branch::translate(const Vector2d& delta) {
  for (auto& segment : _body) {
    segment.translate(delta);
  }
  for (auto& branch : _branches) {
    branch.translate(delta);
  }
}

bool Tamarak::Model::Branch::operator==(const Branch& other) const {
  return _body == other._body && _branches == other._branches;
}

bool Tamarak::Model::Branch::operator!=(const Branch& other) const { return !(*this == other); }
