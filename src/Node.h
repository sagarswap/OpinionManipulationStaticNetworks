#ifndef NODE_H
#define NODE_H

#include <string>
#include <vector>
#include <iostream>
#include <random>
#include <fstream>
#include <sstream>

class Edge;  // Forward declaration

class Node {
    int nodeId, malice; //0 -> Real Person, 1 -> Bot, 2 -> Troll
    bool malicious; //false = Not malicious, i.e. Real Person, true = Malicious Node
    bool state;
    std::vector<Edge*> neighbours; // List of edges connected to this node
public:
    Node(int nodeId, int bot, bool state);
    int getId() const;
    bool getState() const;
    std::vector<Edge*> getNeighbours() const;
    int getMalice() const;
    void changeState();
    void addNeighbour(Edge* edge);
    void printNeighbours();
    bool isNeighbour(Node* node);
    bool isActiveNeighbour(Node* node);
    void activateEdge(Node* node);
    void deactivateEdge(Node* node);
    bool isMalicious() const;
    bool hasInactiveEdge() const;
    bool hasActiveDiscordantEdge() const;
    long getActiveDiscordantEdgeCount() const;
    Node* getRandomActiveDiscordantEdgeNode();
    Edge* getRandomHarmoniousInactiveEdge() const;

    int getRandomNumber(int limit) const;
};

#endif // NODE_H