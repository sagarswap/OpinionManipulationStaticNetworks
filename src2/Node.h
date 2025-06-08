#ifndef NODE_H
#define NODE_H

#include <string>
#include <vector>
#include <iostream>
#include <random>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <utility>

class Edge;  // Forward declaration

class Node {
    int id, malice; //TODO: Can use boolean for malice if just studiying bots
    bool state;
    std::unordered_map<int, std::pair<Node*, bool>> neighbours;
    std::unordered_map<int, Edge*> edges;

public:
    Node(int identity, bool status, int malice);
    bool getState() const;
    int getId() const;
    int getMalice() const; //rename function to isBot
    std::unordered_map<int, std::pair<Node*, bool>> getNeighbours() const;
    void setState(bool status);
    void changeState();
    void addNeighbour(Node* node, Edge* edge);
    void inactivateEdge(int id);
    void inactivateEdge(Node* node);
    void activateEdge(int id);
    void activateEdge(Node* node);
    void printAllNeighbours() const;
    void printAllNeighbours(int limit) const;
    bool isNeighbour(int id) const;
    bool isNeighbour(Node* node) const;
    bool isActiveNeighbour(int id);
    bool isActiveNeighbour(Node* node);
    bool hasInactiveEdge() const;
    Node* getRandomInactiveEdge() const;
    Node* getRandomInactiveZeroStateEdge() const;

    //util
    int getRandomNumber(int limit) const;
};

#endif // NODE_H