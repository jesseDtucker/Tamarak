#include "Trunk.hpp"

using namespace std;
using namespace Tamarak::Model;

Trunk::Trunk(const vector<Segment>& body) : _body(body) {}

bool Tamarak::Model::Trunk::operator==(const Trunk& other) const { return _body == other._body; }

bool Tamarak::Model::Trunk::operator!=(const Trunk& other) const { return !(*this == other); }
