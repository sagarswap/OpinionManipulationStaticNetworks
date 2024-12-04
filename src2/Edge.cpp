#include "Node.h"
#include "Edge.h"

Edge::Edge(Node* A, Node* B){
    this->nodeA=A;
    this->nodeB=B;
    this->active=true;
    this->nodeA->addNeighbour(B, this);
    this->nodeB->addNeighbour(A, this);
}

bool Edge::getStatus() const {
    return this->active;
}

void Edge::setStatus(bool status) {
    this->active=status;
}

Node* Edge::getOtherNode(Node* node) const {
    if(node==nodeA)
        return nodeB;
    else if(node==nodeB)
        return nodeA;
    return nullptr;
}

Node* Edge::getNodeA() const {
    return this->nodeA;
}

Node* Edge::getNodeB() const {
    return this-> nodeB;
}

bool Edge::isDiscordant() const {
    if(this->nodeA->getState()!=this->nodeB->getState())
        return true;
    return false;
}