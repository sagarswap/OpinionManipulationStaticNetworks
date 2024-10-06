#include "Edge.h"
#include "Node.h"

// Constructor to initialize an edge between two nodes
Edge::Edge(Node* fromNode, Node* toNode) : from(fromNode), to(toNode) {}