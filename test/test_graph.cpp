#include "../src/Edge.h"
#include "../src/Node.h"
#include "../src/Graph.h"
#include <iostream>
#include <cassert>
#include <vector>
#include <algorithm>

int main(){
    Graph* g=new Graph("input/RealWorld/trial.txt", 0.5, 0.5, 0, 0); //Plain graph without any malicious users
    g->loadData();
    Node* n1=g->getNode(0);
    Node* n2=g->getNode(1);
    Node* n3=g->getNode(2);
    Node* n4=g->getNode(3);
    Node* n5=g->getNode(4);
    Node* n6=g->getNode(5);

    //Test node creation
    assert(g->nodeList.size()==6);
    for(int i=0; i<g->nodeList.size(); i++){
        Node* n=g->getNode(i);
        assert(n->getId()==i);
        auto it= std::find(g->nodeList.begin(), g->nodeList.end(), n);
        assert(it!=g->nodeList.end());
    }
    std::cout<<"Test Case 1 Passed: All nodes created"<<std::endl;
    
    //Test edge creation
    Edge* e1=g->getEdge(n1, n2);
    Edge* e2=g->getEdge(n1, n3);
    Edge* e3=g->getEdge(n1, n4);
    Edge* e4=g->getEdge(n1, n5);
    Edge* e5=g->getEdge(n1, n6);
    Edge* e6=g->getEdge(n2, n3);
    Edge* e7=g->getEdge(n2, n5);
    Edge* e8=g->getEdge(n3, n5);
    Edge* e9=g->getEdge(n3, n6);
    Edge* e10=g->getEdge(n3, n4);
    Edge* e11=g->getEdge(n5, n6);
    Edge* e12=g->getEdge(n4, n5);
    Edge* e13=g->getEdge(n4, n6);
    assert(g->edgeList.size()==13);
    assert(e1!=nullptr && e2!=nullptr && e3!=nullptr && e4!=nullptr && e5!=nullptr && 
            e6!=nullptr && e7!=nullptr && e8!=nullptr && e9!=nullptr && e10!=nullptr 
            && e11!=nullptr && e12!=nullptr && e13!=nullptr);
    std::cout<<"Test Case 2 Passed: All edges created"<<std::endl;
    
    //Test neighbour initialization
    assert(n1->getNeighbours().size()==5);
    assert(n2->getNeighbours().size()==3);
    assert(n3->getNeighbours().size()==5);
    assert(n4->getNeighbours().size()==4);
    assert(n5->getNeighbours().size()==5);
    assert(n6->getNeighbours().size()==4);
    assert(n1->isNeighbour(n2));
    assert(n1->isNeighbour(n3));
    assert(n1->isNeighbour(n4));
    assert(n1->isNeighbour(n5));
    assert(n1->isNeighbour(n6));
    assert(n2->isNeighbour(n1));
    assert(n2->isNeighbour(n3));
    assert(n2->isNeighbour(n5));
    assert(!n2->isNeighbour(n4));
    assert(!n2->isNeighbour(n6));
    assert(n3->isNeighbour(n1));
    assert(n3->isNeighbour(n2));
    assert(n3->isNeighbour(n4));
    assert(n3->isNeighbour(n5));
    assert(n4->isNeighbour(n1));
    assert(n4->isNeighbour(n3));
    assert(n4->isNeighbour(n5));
    assert(n4->isNeighbour(n6));
    assert(!n4->isNeighbour(n2));
    assert(n5->isNeighbour(n1));
    assert(n5->isNeighbour(n2));
    assert(n5->isNeighbour(n3));
    assert(n5->isNeighbour(n4));
    assert(n5->isNeighbour(n6));
    assert(n6->isNeighbour(n1));
    assert(n6->isNeighbour(n3));
    assert(n6->isNeighbour(n4));
    assert(n6->isNeighbour(n5));
    assert(!n6->isNeighbour(n2));
    std::cout<<"Test Case 3 Passed: Neighbours created properly"<<std::endl;

    //Test if inactive edges were created
    bool hasInactive=false;
    for(Edge* e: g->edgeList){
        if(!e->isActive())
            hasInactive=true;
    }
    assert(hasInactive);
    std::cout<<"Test Case 4 Passed: Inactive edges created"<<std::endl;

    g->setEdgeLists();

    //Test getRandomInactiveEdge()
    Edge* e=g->getRandomInactiveEdge();
    if(e!=nullptr)
        assert(!e->isActive());
    assert(g->inactiveEdges.find(e->edgeName) == g->inactiveEdges.end());
    auto it2 = std::find(g->inactiveEdgesStr.begin(), g->inactiveEdgesStr.end(), e->edgeName);
    assert(it2 == g->inactiveEdgesStr.end());
    std::cout<<"Test Case 5 Passed: getRandomInactiveEdge()"<<std::endl;
    
    //Test getRandomActiveDiscordantEdge()
    e=g->getRandomActiveDiscordantEdge();
    if(e!=nullptr){
        assert(e->isActive());
        assert(e->isDiscordant());
    }
    assert(g->aDiscordantEdges.find(e->edgeName) == g->aDiscordantEdges.end());
    auto it = std::find(g->aDiscordantEdgesStr.begin(), g->aDiscordantEdgesStr.end(), e->edgeName);
    assert(it == g->aDiscordantEdgesStr.end());
    std::cout<<"Test Case 6 Passed: getRandomActiveDiscordantEdge()"<<std::endl;

    //Test for uniform distribution of nubers via random number generator
    int ar[]={0, 0, 0, 0, 0};
    for(int i=0;i<1000; i++)
        ar[g->getRandomNumber(4)]++;
    assert(ar[0]>50);
    assert(ar[1]>50);
    assert(ar[2]>50);
    assert(ar[3]>50);
    assert(ar[4]>50);
    std::cout<<"Test Case 7 Passed: Random number generator is uniform"<<std::endl;
}