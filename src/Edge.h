#ifndef EDGE_H
#define EDGE_H

#include <iostream>
#include <algorithm>

class Node;  // Forward declaration of Node

class Edge {
    bool active;
public:
    Node* nodeA;
    Node* nodeB;
    Edge(Node* nA, Node* nB);
    std::string edgeName;

    void activateEdge();
    void deactivateEdge();
    bool isActive() const;
    Node* getOtherNode(Node* n) const;
    bool isDiscordant() const;
    Node* getNodeWithInactiveEdge() const;
    int getMaliciousness() const;
    Node* getMaliciousNode() const;
    Node* getRealNode() const;

    double getRandomNumber() const;
};

#endif // EDGE_H