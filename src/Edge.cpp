#include "Edge.h"
#include "Node.h"

// Constructor to initialize an edge between two nodes
Edge::Edge(Node* nA, Node* nB) {
    this->nodeA=nA;
    this->nodeB=nB;
    active=true;
}

void Edge::activateEdge(){
    active=true;
}

void Edge::deactivateEdge(){
    active=false;
}

bool Edge::isActive() const{
    return active;
}

Node* Edge::getOtherNode(Node* n) const {
    if (n==this->nodeA) 
        return this->nodeB;
    return this->nodeA;
}

bool Edge::isDiscordant() const {
    if(this->nodeA->getState()!=this->nodeB->getState())
        return true;
    return false;
} 