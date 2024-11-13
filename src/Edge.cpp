#include "Edge.h"
#include "Node.h"

// Constructor to initialize an edge between two nodes
Edge::Edge(Node* nA, Node* nB)
    : nodeA(nA), nodeB(nB), active(true) { 
        nA->addNeighbour(this);
        nB->addNeighbour(this);
        this->edgeName=std::to_string(nA->getId())+"_"+std::to_string(nB->getId());
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

Node* Edge::getNodeWithInactiveEdge() const {
    if(this->nodeA->hasInactiveEdge() && this->nodeB->hasInactiveEdge()){
        if(this->getRandomNumber()<0.5)
            return this->nodeA;
        else
            return this->nodeB;
    }
    else if(this->nodeA->hasInactiveEdge())
        return this->nodeA;
    else if(this->nodeB->hasInactiveEdge())
        return this->nodeB;
    else
        return nullptr;
}

int Edge::getMaliciousness() const{
    return std::max(this->nodeA->getMalice(), this->nodeB->getMalice());
}

Node* Edge::getMaliciousNode() const {
    if(this->nodeA->isMalicious())
        return nodeA;
    else if(this->nodeB->isMalicious())
        return nodeB;
    else
        return nullptr;
}

Node* Edge::getRealNode() const {
    if(!this->nodeA->isMalicious() && !this->nodeB->isMalicious())
        return nullptr;
    else if(!this->nodeA->isMalicious())
        return nodeA;
    else if(!this->nodeB->isMalicious())
        return nodeB;
    else
        return nullptr;
}

double Edge::getRandomNumber() const {
    std::random_device rd;
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    return dist(rd);
}