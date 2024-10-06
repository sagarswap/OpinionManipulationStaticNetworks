#include "Graph.h"
#include <iostream>

// Destructor to clean up all dynamically allocated nodes
Graph::~Graph() {
    for (auto& pair : nodes) {
        delete pair.second;  // Delete each node
    }
}

// Add a new node to the graph
void Graph::addNode(const std::string& nodeName) {
    if (nodes.find(nodeName) == nodes.end()) {
        nodes[nodeName] = new Node(nodeName);
    } else {
        std::cerr << "Node " << nodeName << " already exists." << std::endl;
    }
}

// Add an edge between two nodes
void Graph::addEdge(const std::string& from, const std::string& to) {
    if (nodes.find(from) == nodes.end() || nodes.find(to) == nodes.end()) {
        std::cerr << "One or both nodes not found." << std::endl;
        return;
    }

    Node* fromNode = nodes[from];
    Node* toNode = nodes[to];

    // Create a new edge and add it to the from-node's list of edges
    Edge* edge = new Edge(fromNode, toNode);
    fromNode->addEdge(edge);
}

// Display all nodes and their edges
void Graph::displayGraph() const {
    std::cout << "Graph:" << std::endl;
    for (const auto& pair : nodes) {
        pair.second->printEdges();
    }
}
