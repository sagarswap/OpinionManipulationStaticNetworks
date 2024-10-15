#include "Graph.h"

Graph::Graph(std::string fName, double rewire, double sRatio, double mRatio, int malice){
    this->inputFileName=fName;
    this->rewiringProbability=rewire;
    this->startRatio=sRatio;
    this->epochLimit=100000;
    this->stepCount=100;
    this->inputFileName=fName;
    this->maliciuousRatio=mRatio;
    this->maliceType=malice;
    this->stat0=0, stat1=0;

    if(fName.substr(0, 5)=="RealW"){
        std::string fname=fName.substr(10);
        this->inputFileName="../input/"+fName+".txt";
        this->outputFileName="../output/bots/"+fName+"/s"+std::to_string(sRatio)+"_r"+std::to_string(rewire)+"_m"+std::to_string(mRatio)+"_"+std::to_string(getRandomNumber(10000))+".txt";
    }
    else if(fName.substr(0, 5)=="Watts"){
        std::string pVal=fName.substr(fName.size()-2, 2)+"/";
        this->inputFileName="../input/"+fName+".txt";
        this->outputFileName="../output/bots/"+fName+"/s"+std::to_string(sRatio)+"_r"+std::to_string(rewire)+"_m"+std::to_string(mRatio)+"_p"+pVal+"_"+std::to_string(getRandomNumber(10000))+".txt";
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
            generateNetwork(inputNode, outputNode);
            cc++;
        }
        else{
            if(inputNode==outputNode)
                continue; //prevents edges into self
            Node* node1=getNode(inputNode);
            Node* node2=getNode(outputNode);
            Edge* edge=new Edge(node1, node2);
            edgeList.push_back(edge);
            node1->addNeighbour(edge);
            node2->addNeighbour(edge);
        }        
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
    this->avgMaliciousEdgeCount=(int)(edges/nodes);
    for (int i = 0; i < nodes; i++) {
        if(this->getRandomNumber()<this->startRatio){
            nodeList.push_back(new Node(i, false, 0));
            stat0++;
        }
        else{
            nodeList.push_back(new Node(i, true, 0));
            stat1++;
        }
    }
}

void Graph::generateSubNetwork(){
    for(auto& edge: edgeList)
        if(this->getRandomNumber()>0.8)
            edge->deactivateEdge();
}

void Graph::beginSimulation(){
    this->setEdgeLists();
    bool altEdgeSelectionAlgo=false;
    std::cout<<this->getSummary(-1)<<std::endl;
    std::ofstream outputFile;
    outputFile.open(this->outputFileName);
    outputFile<<"Epoch Pop Frac DiscEdge"<<std::endl;
    int epoch=0;
    for(; epoch<this->epochLimit && this->aDiscordantEdges.size()>0; epoch++){
        for(int step=0; step< this->stepCount && this->aDiscordantEdges.size()>0; step++){
            int edgeIndex=this->getRandomNumber(this->aDiscordantEdges.size()-1);
            Edge* curEdge=aDiscordantEdges[edgeIndex];
            double decide=this->getRandomNumber();
            if(decide<=this->rewiringProbability)
                this->rewire(curEdge, edgeIndex);
            else
                this->convince(curEdge, edgeIndex);

        }
        std::string summary=getSummary(epoch);
        std::cout<<summary<<std::endl;
        outputFile << summary <<std::endl;
    }
    outputFile << getSummary(epoch) << std::endl;
    outputFile.close();
}

void Graph::rewire(Edge* edge, int edgeIndex){
    Node* node=edge->getNodeWithInactiveEdge();
    if(node==nullptr)
        return;
    Edge* newEdge=node->getRandomInactiveEdge();
    newEdge->activateEdge();
    this->aDiscordantEdges.erase(this->aDiscordantEdges.begin()+edgeIndex);
    if(newEdge->isDiscordant())
        this->aDiscordantEdges.push_back(newEdge);
}

void Graph::convince(Edge* edge, int edgeIndex){
    Node* node; //The node who will change its opinion
    int m=edge->getMaliciousness();
    if(m==0){
        if(this->getRandomNumber()<0.5)
            node=edge->nodeA;
        else
            node=edge->nodeB;
        node->changeState();
    }
    else if(m==1){
        node=edge->getRealNode(); //Bot will always convince the real person to change their state.
        node->changeState();
    }
    else if(m==4){
        node=edge->getMaliciousNode(); //The Cyborg will convince all the real person it is connected to, to switch their state to him
        std::vector<Edge*> maliciousEdges=node->getNeighbours();
        for(Edge* e: maliciousEdges){
            if(e->isDiscordant() && e->isActive()){
                Node* n=e->getRealNode();
                n->changeState();
            }
        }
    }
    this->setEdgeLists();
}

Node* Graph::getNode(int id) const {
    if(id<0 || id>=this->nodeCount){
        std::cout<<"Invalid node ID"<<std::endl;
        return nullptr;
    }
    return this->nodeList[id];
}

void Graph::addMaliciousUsers() {
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

Edge* Graph::getRandomInactiveEdge() const {
    return this->inactiveEdges[this->getRandomNumber(this->inactiveEdges.size()-1)];
}

long Graph::getActiveDiscordantEdgeCount() const {
    long count=0;
    for(const auto& edge: this->edgeList){
        if(edge->isDiscordant() && edge->isActive())
            count++;
    }
    return count;
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