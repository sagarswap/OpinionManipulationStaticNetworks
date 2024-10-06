#ifndef GRAPH_H
#define GRAPH_H

#include <unordered_map>
#include <string>

#include "Node.h"
#include "Edge.h"

class Graph {
private:
    std::unordered_map<std::string, Node*> nodes;

public:
    ~Graph();  // Destructor to clean up allocated memory
    void addNode(const std::string& nodeName);
    void addEdge(const std::string& from, const std::string& to);
    void displayGraph() const;
};

#endif // GRAPH_H
