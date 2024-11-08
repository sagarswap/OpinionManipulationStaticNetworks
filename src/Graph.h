#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <list>
#include <random>
#include <string>


#include "Node.h"
#include "Edge.h"

class Graph {
public: //Set public for testing, else keep private
    std::vector<Node*> nodeList;
    std::vector<Edge*> edgeList;
    std::list<Edge*> aDiscordantEdges;
    std::list<Edge*> inactiveEdges;
    std::string inputFileName, outputFileName;
    long nodeCount, edgeCount, stat0, stat1, epochLimit, stepCount;
    double rewiringProbability, relativeSize, startRatio, maliciuousRatio;
    int avgMaliciousEdgeCount, maliceType;

//public: //always public
    Graph(std::string fName, double rewire, double sRatio, double mRatio, int malice);
    ~Graph();  // Destructor to clean up allocated memory
    void loadData();
    void generateNetwork(int nodes, int edges);
    void generateSubnetwork();
    void beginSimulation();
    void displayGraph() const;
    Node* getNode(int id) const;
    Edge* getEdge(Node* a, Node* b) const;
    void generateSubNetwork();
    void addMaliciousUsers();
    long getActiveDiscordantEdgeCount() const;
    void setEdgeLists();
    void updateEdgeLists(Node* node);
    void rewire(Edge* edge);
    void convince(Edge* edge);
    Edge* getRandomInactiveEdge() const;
    Edge* getRandomActiveDiscordantEdge() const;
    
    //Util
    std::string getSummary(int epoch);
    int getRandomNumber(int limit) const;
    double getRandomNumber() const;
    Node* getRandomNode() const;
    std::string getMaliciousFileName() const;

    //Debugging
    void addNode(Node* node);
    void addEdge(Edge* edge);
    Graph();
};

#endif // GRAPH_H
