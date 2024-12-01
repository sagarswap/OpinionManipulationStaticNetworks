#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <list>
#include <random>
#include <string>
#include <unordered_map>

#include "Node.h"
#include "Edge.h"

class Graph {
    int nodeCount, edgeCount, stat0, stat1, malice;
    int epochLimit, stepCount;
    double rewiringProbability, relativeSize, startingRatio, maliceRatio;
    std::vector<Node*> nodeList;
    std::vector<Edge*> edgeList;
    std::string inputFileName, outputFileName;
    long rew=0, con=0;
public:
    Graph(std::string infname, double rewire, double startRatio, double maliciousRatio, int malicious);
    void loadData();
    void generateNetwork(int nodeC, int edgeC);
    void generateSubNetwork();
    void beginSimulation();
    bool interact();
    bool interactAlt();
    void convince(Node* inputNode, Node* outputNode);
    void rewire(Node* adderNode, Node* deleterNode);
    Node* getNode(int identity) const;
    void inactivateEdge(Edge* edge);
    void activateEdge(Edge* edge);
    long getActiveDiscordantEdgeCount() const;
    bool hasActiveDiscordantEdge(Node* node) const;
    Node* getRandomActiveDiscordantEdge(Node* node) const;

    //util
    void recountStates() const;
    int getRandomNumber(int limit) const;
    double getRandomNumber() const;
    Node* getRandomNode() const;
    void printAllEdges(int lim) const;
    void printAllEdges() const;
    std::string getSummary(int epoch, long discEdge) const;
    std::string getSubFolderName(double ratio) const;
};

#endif // GRAPH_H