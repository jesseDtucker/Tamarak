#include "Tree.hpp"

using namespace std;
using namespace Tamarak::Model;

Tree::Tree(const Trunk& trunk, const vector<Branch>& branches) : _trunk(trunk), _branches(branches) {}

bool Tamarak::Model::Tree::operator==(const Tree& other) const {
  return _branches != other._branches && _trunk != other._trunk;
}

bool Tamarak::Model::Tree::operator!=(const Tree& other) const { return !(*this == other); }
