#include "Node.h"
#include "Edge.h"
#include "Graph.h"

Graph::Graph(std::string infname, double rewire, double startRatio, double maliciousRatio, int malicious){
    std::cout<<"Constructor reached"<<std::endl;
    int n1=rewire*100.0;
    int n2=maliciousRatio*100.0;
    this->startingRatio=startRatio;
    this->malice=malicious;
    this->maliceRatio=maliciousRatio;
    std::string sub=getSubFolderName();
    if(infname.substr(0, 5)=="RealW"){
        std::string fname=infname.substr(10);
        this->inputFileName="../input/"+infname+".txt";
        this->outputFileName="../output/RealWorld/"+fname+sub+"_r"+std::to_string(n1)+"_m"+std::to_string(n2)+"_"+std::to_string(getRandomNumber(10000))+".txt";
    }
    else if(infname.substr(0, 5)=="Watts"){
        std::string pVal=infname.substr(infname.size()-2, 2)+"/";
        this->inputFileName="../input/WattsStrogatz/"+infname+".txt";
        this->outputFileName="../output/WattsStrogatz"+sub+pVal+"ws_r"+std::to_string(n1)+"_m"+std::to_string(n2)+"_"+std::to_string(getRandomNumber(10000))+".txt";
    }
    stat0=0;
    stat1=0;
    this->epochLimit=INT32_MAX;
    this->stepCount=250 ;
    this->rewiringProbability=rewire; 
    this->relativeSize=0.5;
    std::cout<<outputFileName<<std::endl;
}

/**
 * This function reads the text file which was defined while creating the object of ComplexNetwork. 
 * The first line of the file contains the nodeCount and the edgeCount.
 * It calls generateNetwork() which creates the network and sets up other parameters.
 * This function then connects the entire network by defining the edges between the nodes.
*/
void Graph::loadData(){
    std::cout<<"Data Load Start"<<std::endl;
    std::fstream file;
    file.open(this->inputFileName, std::ios::in);
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
        else{
            if(inputNode==outputNode)
                continue; //prevents edges into self
            Node* node1=getNode(inputNode);
            Node* node2=getNode(outputNode);
            Edge* edge=new Edge(node1, node2);
            this->edgeList.push_back(edge);
        }        
    }
    std::cout<<"Data Loaded"<<std::endl;
    file.close();
    generateSubNetwork();
    std::cout<<"Nodes = "<<this->nodeCount<<"\tEdges = "<<this->edgeCount<<std::endl;
}

/**
 * This function initialises the values of nodeCount, edgeCount, stat0 and stat1, creates every Node and assigns a random state to each one of them.
 * Input Parameters :   nodeC - An integer variable containing the number of nodes in the network.
 *                      edgeC - An integer variable containing the number of edges in the network.
*/
void Graph::generateNetwork(int nodeC, int edgeC){
    this->nodeCount=nodeC;
    this->edgeCount=edgeC;
    for(int i=1; i<=nodeC; i++){
        double randState=getRandomNumber();
        Node* newNode;
        if(randState<this->startingRatio){
            stat0++;
            newNode=new Node(i, false, 0);
        }
        else{
            stat1++;
            newNode=new Node(i, true, 0);
        }
        
        this->nodeList.push_back(newNode);
    }
    std::cout<<"Nodes Generated"<<std::endl;
}

void Graph::generateSubNetwork(){
    for(Edge* edge: edgeList){
        double rando=this->getRandomNumber();
        if(relativeSize<rando){
            this->inactivateEdge(edge);
        }
    }
    std::cout<<"Subnetwork Generated"<<std::endl;
}

/**
 * This function starts the simulation by calling the interact() as well as regularly printing the results after every stepCount loop.
 * Input Parameters :   epochLimit - An integer variable which defines the duration of execution of the simulation.
 *                      stepCount - An integer variable which defines the number of steps in each epoch. The total duration of the simulation = epochLimit*stepCount.
 *                      rewiringProbability - The frequency with which nodes ineract with each other.
*/
void Graph::beginSimulation(){
    long discEdge=this->getActiveDiscordantEdgeCount();
    bool altEdgeSelectionAlgo=false;
    std::cout<<getSummary(-1, discEdge)<<std::endl;
    std::ofstream outputFile;
    outputFile.open(this->outputFileName);
    outputFile<<"Epoch Pop Frac DiscEdge"<<std::endl;
    for(int epoch=0; epoch<this->epochLimit; epoch++){
        for(int step=0; step<this->stepCount; step++){
            bool areWeDone=false;
            if(!altEdgeSelectionAlgo)
                altEdgeSelectionAlgo=interact();
            else
                areWeDone=interactAlt();
            if(areWeDone){
                discEdge=this->getActiveDiscordantEdgeCount();
                std::string summary=getSummary(epoch, discEdge);
                std::cout<<summary<<std::endl;
                outputFile << summary << std::endl;
                this->recountStates();
                std::cout<<"Simulation Completed!"<< std::endl;
                outputFile.close();
                return;
            }
        }
        if(epoch%10==0){
            std::cout << "Epoch No. " << epoch << std::endl;
        }
        discEdge=this->getActiveDiscordantEdgeCount();
        if(discEdge==0){
            std::string summary=getSummary(epoch, discEdge);
            std::cout<<summary<<std::endl;
            outputFile << summary <<std::endl;
            this->recountStates();
            std::cout<<"Simulation Completed!"<<std::endl;
            outputFile.close();
            return;
        }
        std::string summary=getSummary(epoch, discEdge);
        std::cout<<summary<<std::endl;
        outputFile << summary <<std::endl;
    }
    outputFile.close();
}

/**
 * This function is responsible for the interactions between the nodes, which are further controlled by convince() and rewire().
 * It iterates through every edge and uses random number generation to decide on its interaction
 * Input Parameter - An integer variable between 0 and 0.1 which controls how often an interaction occurs. 
*/
bool Graph::interact(){
    bool ideal=false;
    double rando=this->getRandomNumber();
    Node* node;
    int tries=0;
    do{
        rando=this->getRandomNumber();
        node=this->getRandomNode();
        if(rando<=rewiringProbability){ //check viability for rewiring
            if(node->hasInactiveEdge() && this->hasActiveDiscordantEdge(node))
                ideal=true;
        }
        else{ //check viability for convincing
            if(this->hasActiveDiscordantEdge(node))
                ideal=true;
        }
        tries++;
    }while(!ideal && tries<10000);
    
    if(tries>=10000){
        std::cout<<"Tries is greater with a value of "<<tries<<std::endl;
        return true;
    }

    if(rando<=rewiringProbability){
        Node* neighbour=this->getRandomActiveDiscordantEdge(node);
        this->rewire(node, neighbour);
    }
    else{ 
        Node* neighbour=this->getRandomActiveDiscordantEdge(node);
        this->convince(node, neighbour);
    }
    return false;
}

bool Graph::interactAlt(){
    std::vector<Edge*> roster;
    double rando=this->getRandomNumber();
    for(Edge* edge: edgeList){
        if(edge->isDiscordant() && edge->getStatus()){
            if(rando<=this->rewiringProbability){
                if(edge->getNodeA()->hasInactiveEdge() || edge->getNodeB()->hasInactiveEdge()) //for rewiring
                    roster.push_back(edge);
            }
            else //for convincing
                    roster.push_back(edge);
        }
    }
    if(roster.size()<=1 || this->getActiveDiscordantEdgeCount()<=1){
        this->recountStates();
        return true;
    }
    Edge* e=roster[this->getRandomNumber(roster.size()-1)];
    roster.clear();
    Node* n1;
    Node* n2;
    if(rando<=this->rewiringProbability){
        if(e->getNodeA()->hasInactiveEdge() && e->getNodeB()->hasInactiveEdge()){
            if(this->getRandomNumber()<=0.5){
                n1=e->getNodeA();
                n2=e->getNodeB();
            }
            else{
                n1=e->getNodeB();
                n2=e->getNodeA();
            }
        }
        else if(e->getNodeA()->hasInactiveEdge()){
            n1=e->getNodeA();
            n2=e->getNodeB();
        }
        else{
            n1=e->getNodeB();
            n2=e->getNodeA();
        }
    }
    else{
        if(this->getRandomNumber()<=0.5){
            n1=e->getNodeA();
            n2=e->getNodeB();
        }
        else{
            n1=e->getNodeB();
            n2=e->getNodeA();
        }
    }
    if(rando<=this->rewiringProbability)
        this->rewire(n1, n2);
    else
        this->convince(n1, n2);
    return false;
}

/**
 * This function is used to change the state of the neighbour with which our node interacted with and now hold the same state.
 * Input Parameters :   inputNode - pointer to the input node.
 *                      outputNode - pointer to the output node.
*/
void Graph::convince(Node* inputNode, Node* outputNode){
    outputNode->changeState();
    if(outputNode->getState()){
        this->stat1++;
        this->stat0--;
    }
    else{
        this->stat0++;
        this->stat1--;
    }
}

/**
 * This function is responsible for the deletion of an old edge and the creation of a new edge which happened as a result of interation between 2 nodes of 2 different states.
 * Input Parameters :   inputNode - pointer to the input node.
 *                      outputNode - pointer to the output node.
*/
void Graph::rewire(Node* adderNode, Node* deleterNode){
    Node* inactiveNode=adderNode->getRandomInactiveEdge();
    if(inactiveNode==nullptr){
        return;
    }
    adderNode->activateEdge(inactiveNode->getId());
    inactiveNode->activateEdge(adderNode->getId());
    adderNode->inactivateEdge(deleterNode->getId());
    deleterNode->inactivateEdge(adderNode->getId());
}

/**
 * This function returns the reference to a node.
 * Input Parameter - An integer variable containing the id of the node to be called. It should range between 1 and nodeCount (inclusive)
 * Return Value - A pointer to the node if it exists.
*/
Node* Graph::getNode(int identity) const {
    if(identity<0)
        std::cout<<"Negative node requested"<<std::endl;\
    if(nodeCount>identity)
        return nodeList[identity];
    std::cout<<"Node with id = "<<identity<<" not found"<<std::endl;
    return nullptr;
}

void Graph::inactivateEdge(Edge* edge){
    edge->setStatus(false);
    Node* nodeA=edge->getNodeA();
    Node* nodeB=edge->getNodeB();
    nodeA->inactivateEdge(nodeB->getId());
    nodeB->inactivateEdge(nodeA->getId());
}

void Graph::activateEdge(Edge* edge){
    edge->setStatus(true);
    Node* nodeA=edge->getNodeA();
    Node* nodeB=edge->getNodeB();
    nodeA->activateEdge(nodeB->getId());
    nodeB->activateEdge(nodeA->getId());
}

/**
 * Iterates through all the nodes and gives the number of discordant edges except for nodes which have no inactive edges
 * Retrun : Long Integer containing the count of discordant edges
*/
long Graph::getActiveDiscordantEdgeCount() const{
    long count=0;
    for(Node* node: nodeList){
        if(!node->hasInactiveEdge())
            continue;
        for(const auto& pair:node->getNeighbours()){
            if(!pair.second.second) //Not counting inactive edges
                continue;
            Node* neighbour=pair.second.first;
            if(node->getState()!=neighbour->getState())
                count++;
        }
    }
    return count/2;
}

/**
 * Checks if a node has an active discordant edge.
 * Input Parameter: Node reference of the node you want to check.
 * Return Type: Boolean value True if there is an active discordant edge, else false.
*/
bool Graph::hasActiveDiscordantEdge(Node* node) const {
    for(const auto& pair: node->getNeighbours()){
        if(!pair.second.second)
            continue;
        Node* n=pair.second.first;
        if(n->getState()!=node->getState())
            return true;
    }
    return false;
}

/**
 * Returns the node reference of a node with an active discordant edge with the given node.
 * Input Parameter: Node reference of the node whose neighbour we want.
 * Return Type: Node reference of node with actvie discordant edge.
*/
Node* Graph::getRandomActiveDiscordantEdge(Node* node) const{
    std::vector<Node*> candidates;
    for(const auto& pair: node->getNeighbours()){
        if(!pair.second.second)
            continue;
        if(pair.second.first->getState()!=node->getState())
            candidates.push_back(pair.second.first);
    }
    if(candidates.size()==0){
        std::cout<<"Active discordant edge retrival failed"<<std::endl;
        return nullptr;
    }
    else if(candidates.size()==1)
        return candidates[0];
    else{
        int rand=this->getRandomNumber(candidates.size()-1);
        return candidates[rand];
    }
}

/*This function verifies if our counts of the node states is correct. It will print an error message incase it isn't*/
void Graph::recountStates() const {
    int s1=0, s2=0;
    for(Node* node: nodeList){
        if(node->getState()==0)
            s1++;
        else
            s2++;
    }
    if(s1!=this->stat0 || s2!=this->stat1)
        std::cout<<"Counting Mismatch"<<std::endl;
}

/**
 * This function generates a random number between 0 and limit, and returns it to the caller
 * Input Parameters : limit - Integer variable which will contain the upper limit of random number generation.
 * Return Value : Integer variable of a random number.
*/
int Graph::getRandomNumber(int limit) const{
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

/**
 * This function returns a string containing the number of nodes in each state at that particular time in the network, its opinion density, number of discordant edges and epoch number
*/
std::string Graph::getSummary(int epoch, long discEdge) const {
    std::ostringstream oss;
    oss<<(epoch+1)<<" "<<this->stat0<<" "<<(this->stat0/(this->stat1*1.0+this->stat0*1.0))<<" "<<discEdge;
    return oss.str();
}

std::string Graph::getSubFolderName() const {
    std::string s1, s2;
    switch(this->malice){
        case 0:
            s1="/noMalice/";
            break;
        case 1:
            s1="/bots/";
            break;
        case 2:
            s1="/troll/";
            break;
    }
    switch((int)(this->startingRatio*10)) {
        case 1:
            s2="01/";
            break;
        case 2:
            s2="02/";
            break;
        case 3:
            s2="03/";
            break;
        case 4:
            s2="04/";
            break;
        case 5:
            s2="05/";
            break;
    }

    return s1+s2;
}