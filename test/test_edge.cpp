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

    //Testing activateEdge() and deactivateEdge()
    e1->deactivateEdge();
    assert(!e1->isActive());
    e1->activateEdge();
    assert(e1->isActive());
    std::cout << "Test 1 passed: Edge activation and deactivation." << std::endl;

    //Testing getOtherNode()
    assert(e1->getOtherNode(n1) == n2);
    assert(e1->getOtherNode(n2) == n1);
    std::cout << "Test 2 passed: getOtherNode() method." << std::endl;

    //Testing isDiscordant()
    assert(!e1->isDiscordant());
    assert(e3->isDiscordant());
    std::cout << "Test 3 passed: isDiscordant() method." << std::endl;

    //Testing getNodeWithInactiveEdge()
    assert(e1->getNodeWithInactiveEdge()==nullptr);
    e3->deactivateEdge();
    assert(e1->getNodeWithInactiveEdge()==n1);
    std::cout << "Test 4 passed: getNodeWithInactiveEdge() method." << std::endl;
    e3->activateEdge();

    //Testing getMaliciousness()
    assert(e1->getMaliciousness()==0);
    assert(e4->getMaliciousness()==1);
    std::cout << "Test 5 passed: getMaliciousness() method." << std::endl;

    //Testing getMaliciousNode()
    assert(e1->getMaliciousNode()==nullptr);
    assert(e4->getMaliciousNode()==n4);
    std::cout << "Test 6 passed: getMaliciousNode() method." << std::endl;

    //testing getRealNode()
    assert(e1->getRealNode()==nullptr);
    assert(e4->getRealNode()==n1);
    std::cout << "Test 7 passed: getRealNode() method." << std::endl;

    //testing getEdgeName()
    assert(e1->edgeName=="0_1");
    std::cout << "test 8 passed: getEdgeName property" <<std::endl;
    
    delete n1, n2, n3, n4, e1, e2, e3, e4;
}