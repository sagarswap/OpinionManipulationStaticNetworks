#include "Graph.h"

int main() {
    Graph graph;

    // Adding nodes
    graph.addNode("A");
    graph.addNode("B");
    graph.addNode("C");

    // Adding edges
    graph.addEdge("A", "B");
    graph.addEdge("B", "C");
    graph.addEdge("A", "C");

    // Display graph
    graph.displayGraph();

    return 0;
}
//To complie program: g++ main.cpp Node.cpp Edge.cpp Graph.cpp -o output
//To run program: ./output