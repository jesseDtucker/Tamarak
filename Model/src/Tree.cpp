#include "Tree.hpp"

using namespace std;
using namespace Tamarak::Model;

Tree::Tree(const Trunk& trunk, const vector<Branch>& branches) : _trunk(trunk), _branches(branches) {}

const Trunk& Tree::trunk() const { return _trunk; }

const vector<Branch>& Tree::branches() const { return _branches; }
