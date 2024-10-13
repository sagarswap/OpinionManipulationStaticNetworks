#include "Node.h"
#include "Edge.h"  // Include the Edge class to use Edge*

// Constructor to initialize a node
Node::Node(int nodeId, int mal, bool state) {
    this->nodeId = nodeId;
    this->malice = mal;
    this->state = state;
    if(mal==0)
        this->malicious=false;
    else
        this->malicious=true;
}

bool Node::getState() const{
    return this->state;
}

void Node::changeState() {
    if(!this->isMalicious())
        this->state=!this->state;
}

// Add an edge to this node
void Node::addNeighbour(Edge* edge) {
    neighbours.push_back(edge);
}

// Print all edges connected to this node
void Node::printNeighbours() const {
    std::cout << nodeId << " -> ";
    for (const auto& edge : neighbours) {
        Node* A=edge->nodeA;
        Node* B=edge->nodeB;
        if(this==A)
            std::cout<<A->nodeId<<", ";
        else
            std::cout << B->nodeId <<", ";
    }
    std::cout << std::endl;
}

bool Node::isNeighbour(Node* node) const{
    for(const auto& edge: neighbours){
        Node* n;
        if(edge->nodeA==this)
            n=edge->nodeB;
        else
            n=edge->nodeA;
        if(n == node)
            return true;
    }
    return false;
}

bool Node::isActiveNeighour(Node* node) const{
    for(const auto& edge: neighbours){
        if(edge->isActive()){
            Node* n;
            if(edge->nodeA==this)
                n=edge->nodeB;
            else
                n=edge->nodeA;
            if(n == node)
                return true;
        }
    }
    return false;
}

void Node::activateEdge(Node* node){
    for(auto& edge: neighbours){
        if(edge->isActive())
            continue;
        Node* n;
        if(edge->nodeA==this){
            if(edge->nodeB==node){
                edge->activateEdge();
                return;
            }
        }
        else{
            if(edge->nodeA==node){
                edge->activateEdge();
                return;
            }
        }
    }
    std::cout<<"Edge activation not performed"<<std::endl;
}

void Node::deactivateEdge(Node* node){
    for(auto& edge: neighbours){
        if(!edge->isActive())
            continue;
        Node* n;
        if(edge->nodeA==this){
            if(edge->nodeB==node){
                edge->deactivateEdge();
                return;
            }
        }
        else{
            if(edge->nodeA==node){
                edge->deactivateEdge();
                return;
            }
        }
    }
    std::cout<<"Edge activation not performed"<<std::endl;
}

bool Node::isMalicious() const{
    return this->malicious;
}

bool Node::hasInactiveEdge() const{
    for(auto& edge: neighbours){
        if(!edge->isActive())
            return true;
    }
    return false;
}

bool Node::hasActiveDiscordantEdge() const{
    for(auto& edge: neighbours){
        if(edge->isDiscordant() && edge->isActive())
            return true;
    }
    return false;
}

long Node::getActiveDiscordantEdgeCount() const{
    long count=0;
    for(auto& edge: neighbours){
        if(edge->isDiscordant() && edge->isActive())
            count++;
    }
    return count;
}

Node* Node::getActiveDiscordantEdgeNode() const {
    std::vector<Edge*> candidates;
    for(auto& edge: neighbours){
        if(edge->isDiscordant() && edge->isActive())
            candidates.push_back(edge);
    }
    if(candidates.size()==0){
        std::cout<<"No Active Discordant Edge for node"<<this->nodeId<<std::endl;
        return nullptr;
    }
    else if(candidates.size()==1){
        Node* a=candidates[0]->nodeA;
        Node* b=candidates[0]->nodeB;
        if(this==a)
            return b;
        return a;
    }
    else{
        Edge* e=candidates[this->getRandomNumber(candidates.size()-1)];
        Node* a=e->nodeA;
        Node* b=e->nodeB;
        if(this==a)
            return b;
        return a;
    }
}

int Node::getRandomNumber(int limit) const {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<int> dist(0, limit); 
    return dist(rng);
}