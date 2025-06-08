#include "Node.h"
#include "Edge.h"

Node::Node(int identity, bool status, int malice){
    this->id=identity;
    this->state=status;
    this->malice=malice;
}

bool Node::getState() const{
    return this->state;
}

int Node::getId() const{
    return this->id;
}

int Node::getMalice() const{
    return this->malice;
}

std::unordered_map<int, std::pair<Node*, bool>> Node::getNeighbours() const{
    return this->neighbours;
}

void Node::setState(bool status) {
    this->state=status;
}

void Node::changeState() {
    this->state=!this->state;
}

void Node::addNeighbour(Node* node, Edge* edge) {
    if(!this->isNeighbour(node))
        neighbours[node->getId()]={node, true};
    edges[node->getId()]=edge;
}

void Node::inactivateEdge(int id) {
    if(this->isNeighbour(id))
        this->neighbours[id].second=false;
    this->edges[id]->setStatus(false);
}

void Node::inactivateEdge(Node* node) {
    if(this->isNeighbour(node))
        this->neighbours[node->getId()].second=false;
    this->edges[node->getId()]->setStatus(false);
}

void Node::activateEdge(int id) {
    if(this->isNeighbour(id))
        this->neighbours[id].second=true;
    this->edges[id]->setStatus(true);
}

void Node::activateEdge(Node* node) {
    if(this->isNeighbour(node))
        this->neighbours[node->getId()].second=true;
    this->edges[node->getId()]->setStatus(true);
}

void Node::printAllNeighbours() const {
    for(const auto& [key, val]: neighbours)
        std::cout<< key << " -> ";
    std::cout<<std::endl;
}

void Node::printAllNeighbours(int lim) const {
    int count=0;
    for(const auto& [key, val]: neighbours){
        if(count>=key)
            break;
        std::cout<< key << " -> ";
        count++;
    }
    std::cout<<std::endl;
}

bool Node::isNeighbour(int id) const {
    if(this->neighbours.find(id)!=this->neighbours.end())
        return true;
    else return false;
}

bool Node::isNeighbour(Node* node) const {
    if(this->neighbours.find(node->getId())!=this->neighbours.end())
        return true;
    else return false;
}

bool Node::isActiveNeighbour(int id) {
    if(this->neighbours.find(id)!=this->neighbours.end()){
        if(this->neighbours[id].second)
            return true;
    }
    return false;
}

bool Node::isActiveNeighbour(Node* node) {
    if(this->neighbours.find(node->getId())!=this->neighbours.end()){
        if(this->neighbours[node->getId()].second)
            return true;
    }
    return false;
}

bool Node::hasInactiveEdge() const {
    for(const auto& [key, val]: this->neighbours){
        if(!val.second)
            return true;
    }
    return false;
}

Node* Node::getRandomInactiveEdge() const {
    std::vector<Node*> inactiveEdges;
    for(const auto& [key, val]: this->neighbours){
        if(!val.second )
            inactiveEdges.push_back(val.first);
    }
    if(inactiveEdges.empty()){
        //std::cout<<"No Inactive Edge found in this node for rewiring"<<std::endl;
        return nullptr;
    }
    return inactiveEdges[this->getRandomNumber(inactiveEdges.size()-1)];
}

Node* Node::getRandomInactiveZeroStateEdge() const {
    std::vector<Node*> inactiveEdges;
    for(const auto& [key, val]: this->neighbours){
        if(!val.second && !val.first->getState())
            inactiveEdges.push_back(val.first);
    }
    if(inactiveEdges.empty()){
        //std::cout<<"No Inactive Edge found in this node for rewiring"<<std::endl;
        return nullptr;
    }
    return inactiveEdges[this->getRandomNumber(inactiveEdges.size()-1)];
}

int Node::getRandomNumber(int limit) const{
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<int> dist(0, limit); 
    return dist(rng);
}