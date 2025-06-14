#include "Graph.h"

int main() {
    // Graph* g=new Graph("RealWorld/trial", 0.95, 0.5, 0.5, 1);
    // g->loadData();
    // g->displayGraph();
    //std::cout<<g->getNode(0)->getActiveDiscordantEdgeCount()<<std::endl;
    
    double rewiring[]={0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9};
    double start[]={0.1, 0.2, 0.3, 0.4, 0.5};
    int l1=sizeof(rewiring)/sizeof(rewiring[0]);
    int l2=sizeof(start)/sizeof(start[0]);
    int execution=0;
    for(double st: start){
        for(double rewire: rewiring){
            execution++;
            Graph* graph=new Graph("RealWorld/facebookMedium", rewire, st, 0.0, 0);
            graph->loadData();
            graph->beginSimulation();
            std::cout<<"Completed for start="<<st<<" Execution = "<<execution<<"/"<<l1*l2<<std::endl;
            delete graph;
        }
    }
    return 0;
}
//To complie program: g++ NoBot.cpp Node.cpp Edge.cpp Graph.cpp -o output
//To run program: ./output