#include "Graph.h"

int main() {    
    double rewiring[]={0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9};
    double start[]={0.1, 0.2, 0.3, 0.4, 0.5};
    double bot[]={0.0, 0.025, 0.05, 0.075, 0.1, 0.125, 0.15, 0.175, 0.2};
    int l1=sizeof(rewiring)/sizeof(rewiring[0]);
    int l2=sizeof(start)/sizeof(start[0]);
    int l3=sizeof(bot)/sizeof(bot[0]);
    int execution=0;
    for(double st: start){
        for(double rewire: rewiring){
            for(double bt: bot){
                execution++;
                Graph* graph=new Graph("RealWorld/facebookMedium", rewire, st, bt, 1);
                graph->loadData();
                graph->beginSimulation();
                std::cout<<"Completed for start="<<st<<" Execution = "<<execution<<"/"<<l1*l2*l3<<std::endl;
                delete graph;
            }
        }
    }
    return 0;
}
//To complie program: g++ NoBot.cpp Node.cpp Edge.cpp Graph.cpp -o output
//To run program: ./output