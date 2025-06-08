#include "Graph.h"

int main() {    
    double rewiring[]={0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9};
    double start[]={ 0.1, 0.2, 0.3, 0.4, 0.5};
    double bot[]={0.015, 0.0001, 0.0002}; //you might have to cha ge more often than you think
    int l1=sizeof(rewiring)/sizeof(rewiring[0]);
    int l2=sizeof(start)/sizeof(start[0]);
    int l3=sizeof(bot)/sizeof(bot[0]);
    int execution=0;
    //try to ensure l1*l2*l3 doesn't go beyond 200, else there is a risk of terminal crashing after a few hours
    for(double st: start){
        for(double rewire: rewiring){
            for(double bt: bot){
                execution++;
                Graph* graph=new Graph("RealWorld/facebookMedium", rewire, st, bt, 1);
                graph->loadData();
                graph->beginSimulation();
                std::cout<<"Completed for start="<<st<<" Execution = "<<execution<<"/"<<l1*l2*l3<<std::endl;
                delete graph; //garbage collection, VVIP
            }
        }
    }
    return 0;
}
//To complie program: g++ NoBot.cpp Node.cpp Edge.cpp Graph.cpp -o output
//To run program: ./output