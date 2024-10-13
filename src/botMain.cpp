#include "Graph.h"

int main() {
    Graph* graph=new Graph("RealWorld/facebookArtist", 0.5, 0.5, 0.05);
    graph->loadData();
    graph->displayGraph();
    return 0;
}
//To complie program: g++ botMain.cpp Node.cpp Edge.cpp Graph.cpp -o output
//To run program: ./output