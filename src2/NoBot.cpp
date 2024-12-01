#include "Graph.h"

int main() {
    // Graph* g=new Graph("RealWorld/trial", 0.95, 0.5, 0.5, 1);
    // g->loadData();
    // g->displayGraph();
    //std::cout<<g->getNode(0)->getActiveDiscordantEdgeCount()<<std::endl;
    
    
    Graph* graph=new Graph("RealWorld/facebook", 0.9, 0.6, 0.0, 0);
    graph->loadData();
    graph->beginSimulation();
    return 0;
}
//To complie program: g++ botMain.cpp Node.cpp Edge.cpp Graph.cpp -o output
//To run program: ./output