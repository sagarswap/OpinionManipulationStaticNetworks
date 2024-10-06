#include "Node.h"
#include "Edge.h"  // Include the Edge class to use Edge*

// Constructor to initialize a node
Node::Node(const std::string& nodeName) : name(nodeName) {}

// Add an edge to this node
void Node::addEdge(Edge* edge) {
    edges.push_back(edge);
}

// Print all edges connected to this node
void Node::printEdges() const {
    std::cout << name << " has edges to: ";
    for (const auto& edge : edges) {
        std::cout << edge->to->name << " ";  // Display the target node of each edge
    }
    std::cout << std::endl;
}
