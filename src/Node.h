#ifndef NODE_H
#define NODE_H

#include <string>
#include <vector>
#include <iostream>

class Edge;  // Forward declaration

class Node {
public:
    std::string name;
    std::vector<Edge*> edges; // List of edges connected to this node

    Node(const std::string& nodeName);
    void addEdge(Edge* edge);
    void printEdges() const;
};

#endif // NODE_H