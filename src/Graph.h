#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <random>
#include <string>


#include "Node.h"
#include "Edge.h"

class Graph {
private:
    std::vector<Node*> nodeList;
    std::vector<Edge*> edgeList;
    std::vector<Edge*> aDiscordantEdges;
    std::vector<Edge*> inactiveEdges;
    std::string inputFileName, outputFileName;
    long nodeCount, edgeCount, stat0, stat1, epochLimit, stepCount;
    double rewiringProbability, relativeSize, startRatio, maliciuousRatio;
    int avgMaliciousEdgeCount, maliceType;

public:
    Graph(std::string fName, double rewire, double sRatio, double mRatio, int malice);
    ~Graph();  // Destructor to clean up allocated memory
    void loadData();
    void generateNetwork(int nodes, int edges);
    void generateSubnetwork();
    void beginSimulation();
    void displayGraph() const;
    Node* getNode(int id) const;
    void generateSubNetwork();
    void addMaliciousUsers();
    long getActiveDiscordantEdgeCount() const;
    void setEdgeLists();
    void rewire(Edge* edge, int edgeIndex);
    void convince(Edge* edge, int edgeIndex);
    Edge* getRandomInactiveEdge() const;
    
    std::string getSummary(int epoch);
    int getRandomNumber(int limit) const;
    double getRandomNumber() const;
    Node* getRandomNode() const;
};

#endif // GRAPH_H
