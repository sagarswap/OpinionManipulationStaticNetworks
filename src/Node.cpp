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

int Node::getId() const{
    return nodeId;
}

bool Node::getState() const{
    return this->state;
}

int Node::getMalice() const {
    return this->malice;
}

void Node::changeState() {
    if(!this->isMalicious())
        this->state=!this->state;
}

// Add an edge to this node
void Node::addNeighbour(Edge* edge) {
    if(this==edge->nodeA || this==edge->nodeB)
        neighbours.push_back(edge);
    else
        std::cout<<"Edge has no reference to base node"<<std::endl;
}

// Print all edges connected to this node
void Node::printNeighbours() {
    std::cout << nodeId << "("<<this->state<<")" << " -> ";
    for (const auto& edge : neighbours) {
        Node* n=edge->getOtherNode(this);
        std::cout << n->getId() << ", ";
    }
    std::cout << std::endl;
}

bool Node::isNeighbour(Node* node){
    for(const auto& edge: neighbours){
        Node* n=edge->getOtherNode(this);
        if(n == node)
            return true;
    }
    return false;
}

bool Node::isActiveNeighbour(Node* node){
    for(const auto& edge: neighbours){
        if(edge->isActive()){
            Node* n=edge->getOtherNode(this);
            if(n==node)
                return true;
        }
    }
    return false;
}

void Node::activateEdge(Node* node){
    for(auto& edge: neighbours){
        if(edge->isActive())
            continue;
        Node* n=edge->getOtherNode(this);
        if(n==node)
            edge->activateEdge();
    }
    std::cout<<"Edge activation not performed"<<std::endl;
}

void Node::deactivateEdge(Node* node){
    for(auto& edge: neighbours){
        if(!edge->isActive())
            continue;
        Node* n=edge->getOtherNode(this);
        if(n==node)
            edge->deactivateEdge();
    }
    std::cout<<"Edge de-activation not performed"<<std::endl;
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

Node* Node::getRandomActiveDiscordantEdgeNode() {
    std::vector<Node*> candidates;
    for(auto& edge: neighbours){
        if(edge->isDiscordant() && edge->isActive())
            candidates.push_back(edge->getOtherNode(this));
    }
    if(candidates.size()==0){
        std::cout<<"No Active Discordant Edge for node"<<this->nodeId<<std::endl;
        return nullptr;
    }
    else if(candidates.size()==1)
        return candidates[0];
    else
        return candidates[this->getRandomNumber(candidates.size()-1)];
}

/**Returns a Harmonious inactive edge.
 * If harmonious edge is not present, will return any inactive edge.
*/
Edge* Node::getRandomHarmoniousInactiveEdge() const {
    std::vector<Edge*> candidates;
    for(const auto& edge: neighbours){
        if(!edge->isActive() && !edge->isDiscordant()){
            candidates.push_back(edge);
        }
    }
    if(candidates.size()>1)
        return candidates[this->getRandomNumber(candidates.size()-1)];
    else if( candidates.size()==1)
        return candidates[0];
    else{
        candidates.clear();
        for(const auto& edge: neighbours){
            if(!edge->isActive())
                candidates.push_back(edge);
        }
        if(candidates.size()>1)
            return candidates[this->getRandomNumber(candidates.size()-1)];
        else if(candidates.size()==1)
            return candidates[0];
        else
            return nullptr;
    } 
        
}

std::vector<Edge*> Node::getNeighbours() const {
    return this->neighbours;
}

int Node::getRandomNumber(int limit) const {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<int> dist(0, limit); 
    return dist(rng);
}