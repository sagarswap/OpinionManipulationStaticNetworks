#include "../src/Edge.h"
#include "../src/Node.h"
#include <iostream>
#include <cassert>

int main(){
    Node* n1=new Node(0, 0, true);
    Node* n2=new Node(1, 0, true);
    Node* n3=new Node(2, 0, false);
    Node* n4=new Node(3, 1, false);
    Edge* e1=new Edge(n1, n2);
    Edge* e2=new Edge(n2, n3);
    Edge* e3=new Edge(n3, n1);
    Edge* e4=new Edge(n1, n4);

    //Testing getId(), getState(), getNeighbours() and getMalice()
    assert(n1->getId()==0);
    assert(n1->getState()==true);
    assert(n1->getMalice()==false);
    assert(n1->getNeighbours().size()==3);
    assert(n4->getId()==3);
    assert(n4->getState()==false);
    assert(n4->getMalice()==1);
    assert(n4->getNeighbours().size()==1);
    std::cout<<"Test Case 1 Passed - getId(), getState() and getMalice()"<<std::endl;

    //Testing changeState()
    assert(n1->getState()==true);
    n1->changeState();
    assert(n1->getState()==false);
    n1->changeState();
    assert(n1->getState()==true);
    std::cout<<"Test Case 2 Passed - changeState()"<<std::endl;

    //Test addNeighbour()
    Node* n=new Node(10, 0, false);
    Node* nn=new Node(11, 0, true);
    assert(n->getNeighbours().size()==0);
    Edge* e=new Edge(n, nn);
    assert(n->getNeighbours().size()==1);
    std::cout<<"Test Case 3 Passed - addNeighbour()"<<std::endl;
    delete n, nn, e;

    //Test isNeighbour()
    assert(n1->isNeighbour(n2)==true);
    assert(n2->isNeighbour(n4)==false);
    std::cout<<"Test Case 4 Passed - isNeighbour()"<<std::endl;

    //Test isActiveNeighbour(), activateEdge(), hasInactiveEdge() and deactivaterEdge()
    assert(n1->isActiveNeighbour(n2)==true);
    assert(n2->isActiveNeighbour(n4)==false);
    assert(n1->hasInactiveEdge()==false);
    assert(n4->hasInactiveEdge()==false);
    e4->deactivateEdge();
    assert(n1->hasInactiveEdge()==true);
    assert(n4->hasInactiveEdge()==true);
    assert(n1->isActiveNeighbour(n4)==false);
    e4->activateEdge();
    assert(n1->hasInactiveEdge()==false);
    assert(n4->hasInactiveEdge()==false);
    assert(n1->isActiveNeighbour(n4)==true);
    std::cout<<"Test Case 5 Passed - isActiveNeighbour(), activateEdge(), hasInactiveNeighbour() and deactivateEdge()"<<std::endl;

    //Test isMalicious()
    assert(n1->isMalicious()==false);
    assert(n4->isMalicious()==true);
    std::cout<<"Test Case 6 Passed - isMalicious()"<<std::endl;

    //Test hasActiveDiscordantEdge() and getActiveDiscordantEdgeCount()
    assert(n1->hasActiveDiscordantEdge()==true);
    assert(n2->hasActiveDiscordantEdge()==true);
    e2->deactivateEdge();
    assert(n2->hasActiveDiscordantEdge()==false);
    e2->activateEdge();
    assert(n1->getActiveDiscordantEdgeCount()==2);
    assert(n4->hasActiveDiscordantEdge()==true);
    e4->deactivateEdge();
    assert(n1->getActiveDiscordantEdgeCount()==1);
    assert(n4->hasActiveDiscordantEdge()==false);
    std::cout<<"Test Case 7 Passed - hasActiveDiscordantEdge() and getActiveDiscordantEdgeCount()"<<std::endl;
    e4->activateEdge();

    //Test getRandomActiveDiscordantEdgeNode()
    assert(n2->getRandomActiveDiscordantEdgeNode()==n3);
    assert(n1->getRandomActiveDiscordantEdgeNode()==n3 || n1->getRandomActiveDiscordantEdgeNode()==n4);
    n3->changeState();
    assert(n2->getRandomActiveDiscordantEdgeNode()==nullptr);
    n3->changeState();
    e4->deactivateEdge();
    assert(n1->getRandomActiveDiscordantEdgeNode()==n3);
    assert(n1->getRandomActiveDiscordantEdgeNode()==n3);
    assert(n1->getRandomActiveDiscordantEdgeNode()==n3);
    assert(n1->getRandomActiveDiscordantEdgeNode()==n3);
    e4->activateEdge();
    std::cout<<"Test Case 8 Passed - getRandomActiveDiscordantEdgeNode()"<<std::endl;

    //Test getRandomharmoniousInactiveEdge()
    assert(n1->getRandomHarmoniousInactiveEdge()==nullptr);
    e1->deactivateEdge();
    assert(n1->getRandomHarmoniousInactiveEdge()==e1);
    e1->activateEdge();
    std::cout<<"Test Case 9 Passed - getRandomHarmoniousInactiveEdge()"<<std::endl;

    //Test getRandomNumber(int)
    int ar[]={0,0,0,0,0};
    int length = sizeof(ar) / sizeof(ar[0]);
    for(int i=0;i<500;i++)
        ar[n1->getRandomNumber(length-1)]++;
    assert(ar[0]>0 && ar[1]>0 && ar[2]>0 && ar[3]>0 && ar[4]>0);
    std::cout<<"Test Case 10 Passed - getRandomNumber(int)"<<std::endl;
}