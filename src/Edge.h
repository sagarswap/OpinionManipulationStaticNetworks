#ifndef EDGE_H
#define EDGE_H

#include <iostream>

class Node;  // Forward declaration of Node

class Edge {
public:
    Node* from;
    Node* to;

    Edge(Node* fromNode, Node* toNode);
};

#endif // EDGE_H