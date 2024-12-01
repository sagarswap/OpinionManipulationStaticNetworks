#ifndef EDGE_H
#define EDGE_H

#include <iostream>
#include <algorithm>

class Node;  // Forward declaration of Node

class Edge{
    Node* nodeA;
    Node* nodeB;
    bool active;
    public:
    Edge(Node* A, Node* B);
    bool getStatus() const;
    void setStatus(bool status);
    Node* getOtherNode(Node* node) const;
    Node* getNodeA() const;
    Node* getNodeB() const;
    bool isDiscordant() const;
};

#endif // EDGE_H