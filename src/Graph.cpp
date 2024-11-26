#include "Graph.h"

Graph::Graph(std::string fName, double rewire, double sRatio, double mRatio, int malice){
    this->inputFileName=fName;
    this->rewiringProbability=rewire;
    this->startRatio=sRatio;
    this->epochLimit=100000;
    this->stepCount=500;
    this->inputFileName=fName;
    this->maliciuousRatio=mRatio;
    this->maliceType=malice;
    this->stat0=0, stat1=0;
    std::string malFolName=getMaliciousFileName();

    if(fName.substr(0, 5)=="RealW"){
        std::string fname=fName.substr(10);
        this->inputFileName="../input/"+fName+".txt";
        this->outputFileName="../output/"+fName+"/"+malFolName+"/s"+std::to_string(sRatio)+"_r"+std::to_string(rewire)+"_m"+std::to_string(mRatio)+"_"+std::to_string(getRandomNumber(10000))+".txt";
    }
    else if(fName.substr(0, 5)=="Watts"){
        std::string pVal=fName.substr(fName.size()-2, 2)+"/";
        this->inputFileName="../input/"+fName+".txt";
        this->outputFileName="../output/"+fName+"/"+malFolName+"/s"+std::to_string(sRatio)+"_r"+std::to_string(rewire)+"_m"+std::to_string(mRatio)+"_p"+pVal+"_"+std::to_string(getRandomNumber(10000))+".txt";
    }
}

// Destructor to clean up all dynamically allocated nodes
Graph::~Graph() {
    for (auto& node : nodeList)
        delete node;
    nodeList.clear();
    for (auto& edge : edgeList)
        delete edge; 
    edgeList.clear();
    std::cout << "Graph destructor called, memory cleaned up." << std::endl;
}

void Graph::loadData(){
    std::cout<<"Data Load Start"<<std::endl;
    std::fstream file;
    file.open(this->inputFileName, std::ios::in);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << this->inputFileName << std::endl;
        return;
    }
    std::string tp;
    int cc=0;
    while(getline(file, tp)){
        std::istringstream is(tp);
        int inputNode, outputNode;
        is>>inputNode;
        is>>outputNode;
        if(cc==0){
            std::cout<<"Start Node Generation"<<std::endl;
            this->generateNetwork(inputNode, outputNode);
            cc++;
        }
        else if(inputNode>=0 && outputNode>=0){
            if(inputNode==outputNode)
                continue; //prevents edges into self
            Node* node1=getNode(inputNode);
            Node* node2=getNode(outputNode);
            Edge* edge=new Edge(node1, node2);
            edgeList.push_back(edge);
        }        
        inputNode=-1;
        outputNode=-1;
    }
    std::cout<<"Data Loaded"<<std::endl;
    file.close();
    this->addMaliciousUsers();
    this->generateSubNetwork();
    std::cout<<"Nodes = "<<this->nodeCount<<"\tEdges = "<<this->edgeCount<<std::endl;
}

void Graph::generateNetwork(int nodes, int edges) {
    this->nodeCount=nodes;
    this->edgeCount=edges;
    this->avgMaliciousEdgeCount=(edges/nodes)+1;
    for (int i = 0; i < nodes; i++) {
        if(this->getRandomNumber()<this->startRatio){
            nodeList.push_back(new Node(i, 0, false));
            this->stat0++;
        }
        else{
            nodeList.push_back(new Node(i, 0, true));
            this->stat1++;
        }
    }
}

void Graph::generateSubNetwork(){
    for(auto& edge: edgeList)
        if(this->getRandomNumber()>0.7)
            edge->deactivateEdge();
}

void Graph::beginSimulation(){
    this->setEdgeLists();
    bool altEdgeSelectionAlgo=false;
    std::cout<<"Start"<<this->getSummary(-1)<<std::endl;
    std::ofstream outputFile;
    outputFile.open(this->outputFileName);
    outputFile<<"Epoch Pop Frac DiscEdge"<<std::endl;
    int epoch=0;
    std::cout<<this->aDiscordantEdges.size()<<std::endl;
    for(; epoch<this->epochLimit && this->aDiscordantEdges.size()>0; epoch++){
        for(int step=0; step< this->stepCount && this->aDiscordantEdges.size()>0; step++){
            Edge* curEdge=this->popRandomActiveDiscordantEdge();
            if(this->getRandomNumber()<=this->rewiringProbability)
                this->rewire(curEdge);
            else
                this->convince(curEdge);
        }
        std::string summary=getSummary(epoch);
        std::cout<<summary<<std::endl;
        outputFile << summary <<std::endl;
    }
    outputFile << getSummary(epoch) << std::endl;
    outputFile.close();
}

void Graph::rewire(Edge* edge){
    Node* node=edge->getNodeWithInactiveEdge();
    if(node==nullptr)
        return;
    Edge* newEdge=node->getRandomHarmoniousInactiveEdge();
    if(newEdge!=nullptr){
        this->activateEdgeInGraph(newEdge);
        this->deactivateEdgeInGraph(edge);
    }
}

void Graph::convince(Edge* edge){
    Node* node; //The node who will change its opinion
    int m=edge->getMaliciousness();
    if(m==0){
        if(this->getRandomNumber()<0.5)
            node=edge->nodeA;
        else
            node=edge->nodeB;
        if(node->getState()){
            this->stat0++;
            this->stat1--;
        }
        else{
            this->stat0--;
            this->stat1++;
        }
        node->changeState();
        this->updateEdgeListsAfterConvince(node);
    }
    else if(m==1){
        node=edge->getRealNode(); //Bot will always convince the real person to change their state.
        node->changeState();
        this->stat0++;
        this->stat1--;
        this->updateEdgeListsAfterConvince(node);
    }
    else if(m==2){
        node=edge->getRealNode();
        std::vector<Edge*> victims=node->getNeighbours();
        std::vector<Edge*> candidates;
        for(Edge* edge: victims){
            if(!edge->isDiscordant())
                candidates.push_back(edge);
        }
        Node* fNode=candidates[this->getRandomNumber(candidates.size()-1)]->getOtherNode(node);
        fNode->changeState();
        this->stat0++;
        this->stat1--;
    }
}

Node* Graph::getNode(int id) const {
    if(id<0 || id>=this->nodeCount){
        std::cout<<"Invalid node ID"<<std::endl;
        return nullptr;
    }
    return this->nodeList[id];
}

Edge* Graph::getEdge(Node* a, Node* b) const {
    for(Edge* e: this->edgeList){
        if(e->nodeA==a || e->nodeB==a){
            Node* n=e->getOtherNode(a);
            if(n==b)
                return e;
        }
    }
    return nullptr;
}

void Graph::addMaliciousUsers() {
    if(this->maliceType==0 || this->maliciuousRatio==0.0)
        return;
    int users=(int)(this->nodeCount*this->maliciuousRatio);
    for (int i = 0; i < users; i++) {
        Node* mal=new Node(this->nodeCount+i, this->maliceType, false);
        for(int j=0;j<this->avgMaliciousEdgeCount;j++){
            Node* newNeighbour=this->getRandomNode(); //potential victim for the malicious user
            if(mal->isNeighbour(newNeighbour) || newNeighbour->isMalicious()){
                j--;
                continue;
            }
            Edge* edge=new Edge(mal, newNeighbour);
            edgeList.push_back(edge);
            this->edgeCount++;
        }
        this->nodeList.push_back(mal);
    }
    this->nodeCount+=users;
}

void Graph::setEdgeLists() {
    this->aDiscordantEdges.clear();
    this->inactiveEdges.clear();
    for(const auto& edge: this->edgeList){
        if(edge->isDiscordant() && edge->isActive())
            this->aDiscordantEdges.push_back(edge);
        else if(!edge->isActive())
            this->inactiveEdges.push_back(edge);
    }
}

Edge* Graph::popRandomInactiveEdge() {
    if(this->inactiveEdges.empty())
        return nullptr;
    int index=this->getRandomNumber(this->inactiveEdges.size()-1);
    Edge* e=this->inactiveEdges[index];
    this->inactiveEdges.erase(this->inactiveEdges.begin()+index);
    return e;
}

Edge* Graph::popRandomActiveDiscordantEdge() {
    if(this->aDiscordantEdges.empty())
        return nullptr;
    int index=this->getRandomNumber(this->aDiscordantEdges.size()-1);
    Edge* e=this->aDiscordantEdges[index];
    this->aDiscordantEdges.erase(this->aDiscordantEdges.begin()+index);
    return e;
}

long Graph::getActiveDiscordantEdgeCount() const {
    long count=0;
    for(const auto& edge: this->edgeList){
        if(edge->isDiscordant() && edge->isActive())
            count++;
    }
    return count;
}

void Graph::updateEdgeListsAfterConvince(Node* node){
    std::vector<Edge*> neighbours=node->getNeighbours();
    for(Edge* edge: neighbours){
        if(edge->isActive()){
            if(edge->isDiscordant())
                this->aDiscordantEdges.push_back(edge);
            else{
                this->aDiscordantEdges.erase(
                    std::remove(this->aDiscordantEdges.begin(), this->aDiscordantEdges.end(), edge),
                    this->aDiscordantEdges.end());
            }
        }
    }
}

void Graph::activateEdgeInGraph(Edge* edge){
    if(edge->isActive())
        return;
    edge->activateEdge();
    if(edge->isDiscordant())
        aDiscordantEdges.push_back(edge);
    this->inactiveEdges.erase(
                std::remove(this->inactiveEdges.begin(), this->inactiveEdges.end(), edge),
                this->inactiveEdges.end());
}

void Graph::deactivateEdgeInGraph(Edge* edge){
    if(!edge->isActive())
        return;
    edge->deactivateEdge();
    if(edge->isDiscordant())
        this->aDiscordantEdges.erase(
                    std::remove(this->aDiscordantEdges.begin(), this->aDiscordantEdges.end(), edge),
                    this->aDiscordantEdges.end());
    this->inactiveEdges.push_back(edge);
}

std::string Graph::getSummary(int epoch){
    std::ostringstream oss;
    oss<<(epoch+1)<<" "<<this->stat0<<" "<<(this->stat0/(this->stat1*1.0+this->stat0*1.0))<<" "<<this->aDiscordantEdges.size();
    return oss.str();
}
// Display all nodes and their edges
void Graph::displayGraph() const {
    for (const auto& node : this->nodeList) 
        node->printNeighbours();
}

int Graph::getRandomNumber(int limit) const {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<int> dist(0, limit); 
    return dist(rng);
}

double Graph::getRandomNumber() const {
    std::random_device rd;
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    return dist(rd);
}

Node* Graph::getRandomNode() const {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, this->nodeCount-1);
    Node* node=this->nodeList[dist(gen)];
    if(node==nullptr){
        std::cout<<"getRandomNode() Error "<<dist(gen)<<std::endl;
        return nullptr;
    }
    return node;
}

std::string Graph::getMaliciousFileName() const {
    if(this->maliceType==0)
        return "noMalice";
    else if(this->maliceType==1)
        return "bots";
    else
        return "troll";
}

void Graph::addNode(Node* node){
    this->nodeList.push_back(node);
}

void Graph::addEdge(Edge* edge){
    this->edgeList.push_back(edge);
}

Graph::Graph(){

}