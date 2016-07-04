#include "Trunk.hpp"

using namespace std;
using namespace Tamarak::Model;

Trunk::Trunk(const vector<Segment>& body) : _body(body) {}

const vector<Segment>& Trunk::body() const { return _body; }
