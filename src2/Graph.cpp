#include "Node.h"
#include "Edge.h"
#include "Graph.h"

Graph::Graph(std::string infname, double rewire, double startRatio, double maliciousRatio, int malicious){
    std::cout<<"Constructor reached"<<std::endl;
    int n1=rewire*100.0;
    int n2=maliciousRatio*1000.0;
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
    this->stepCount=200 ;
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
    while(getline(file, tp)){ //this reads the file, one line at a time
        std::istringstream is(tp); //this contains the entire line
        int inputNode, outputNode;
        is>>inputNode; //the first number of the line gets assigned to inputNode
        is>>outputNode; //the second number of the line gets assigned to outputNode
        if(cc==0){ //it will only run for the first line
            std::cout<<"Start Node Generation"<<std::endl;
            this->generateNetwork(inputNode, outputNode); //you pass the node count and edge count here actually, since that's on the first line
            cc++;
        }
        else{ //adds neighbours to all the nodes
            if(inputNode==outputNode)
                continue; //prevents edges into self
            Node* node1=getNode(inputNode);
            Node* node2=getNode(outputNode);
            Edge* edge=new Edge(node1, node2);
            this->edgeList.push_back(edge);
        }        
    }
    std::cout<<"Data Loaded"<<std::endl;
    file.close(); //No need to read input file anymore
    if(this->malice>0)
        this->generateMaliciousNodes();
    this->generateSubNetwork();
    this->saveGraphConfig(true, true);
    this->saveGraphConfig(true, false);
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
        Node* newNode;
        if(getRandomNumber()<this->startingRatio){
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

void Graph::generateMaliciousNodes() {
    int neighbourCount=(int)(this->edgeCount*2/this->nodeCount); //Can be a parameter you study, instead of hardcoding
    for(int i=1; i<(int)(this->nodeCount*maliceRatio)+1; i++){
        Node* bot=new Node(this->nodeCount+i, false, this->malice); //creation of bot object
        this->nodeList.push_back(bot); //adding bot to nodeList
        std::unordered_set<int> roster; //this will store the neighbours of the bots which we will assign randomly
        while(roster.size()<neighbourCount){
            int r=this->getRandomNumber(this->nodeCount-1); //get a random real nodeId
            if(roster.find(r)==roster.end()) //ensures roster object points at the end of the array
                roster.insert(r); //add int r into the roster, appends it at the end
        }
        for(int num: roster){ //we iterate through our roster
            Node* neighbour=this->getNode(num); //obtain the node reference
            Edge* e=new Edge(bot, neighbour); //create the edge between the bot and its victim
            this->edgeList.push_back(e);
        }
    }
    this->nodeCount=nodeList.size();
    this->edgeCount=edgeList.size();
    std::cout<<"Finished generating malicious nodes"<<std::endl;
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
        for(int step=0; step<this->stepCount; step++){ //control step size according to how long the simulations go, keep big step count for a long simulation
            //Start of interactions
            bool areWeDone=false; //this will trigger end of program execution
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
                this->saveGraphConfig(false, true);
                this->saveGraphConfig(false, false);
                return;
            }
            //End of interactions
        }
        //below part executes when after interaction, discordant edge count >0, mostly just for printing on terminal and output file
        if(epoch%10==0){
            std::cout << "Epoch No. " << epoch << std::endl;
        }
        discEdge=this->getActiveDiscordantEdgeCount();
        if(discEdge==0){ //backup for exit condition
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
 * This randomly picks a node and checks if it has an active discordant edge. The algo will keep selecting random nodes until it finds such a node.
 * Input Parameter - An integer variable between 0 and 0.1 which controls how often an interaction occurs. 
 * 
 * Return Value - If it returns false, the first algo will continue to play.
 *                  If it returns true, then we will switch to the 2nd algo
*/
bool Graph::interact(){ //Best case scenario O(1), worst case scenario: doesn't work
    bool ideal=false;
    double rando=this->getRandomNumber(); //decides whether we rewire or convince
    Node* node;
    int tries=0;
    do{
        rando=this->getRandomNumber();
        node=this->getRandomNode();
        if(rando<=rewiringProbability){ //check viability for rewiring
            if(node->hasInactiveEdge() && this->hasActiveDiscordantEdge(node)) //conditions for rewiring
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
    Node* neighbour=this->getRandomActiveDiscordantEdge(node);
    if(rando<=rewiringProbability)
        this->rewire(node, neighbour);
    else
        this->convince(node, neighbour);
    return false;
}

/**
 * We iterate through the entire nodeList and create a list of neighbours which CAN interact, given our policies.
 * 
 * Return Value - If it returns false, the simulation will continue.
 *                  If it returns true, simulation will end.
*/
bool Graph::interactAlt(){ // works in O(N)
    std::vector<std::pair<Node*, Node*>> roster; //this is a list of neighbours like  [(1, 2), (3 ,4), .....]
    double rando=this->getRandomNumber(); //decides whether we rewire or convince
    Node* neighbour;
    for(Node* node: this->nodeList){
        if(rando<=rewiringProbability){ //check viability for rewiring
            if(node->hasInactiveEdge()){
                neighbour=this->getRandomActiveDiscordantEdge(node);
                if(neighbour!=nullptr)
                    roster.push_back(std::make_pair(node, neighbour));
            }
        }
        else{ //check viability for convincing
            neighbour=this->getRandomActiveDiscordantEdge(node);
            if(neighbour!=nullptr)
                roster.push_back(std::make_pair(node, neighbour));
        }
    }
    if(roster.size()<=5){ //you can choose to comment this out if you feel it messes up with your data quality
        std::cout<<"Roster size too small"<<std::endl;
        return true;
    }
    std::pair<Node*, Node*> element=roster[this->getRandomNumber(roster.size()-1)];
    if(rando<=this->rewiringProbability)
        this->rewire(element.first, element.second);
    else
        this->convince(element.first, element.second);
    return false;
}

/**
 * This function is used to change the state of the neighbour with which our node interacted with and now hold the same state.
 * It is expected that this function will always receieve a discordant edge
 * Input Parameters :   inputNode - pointer to the input node.
 *                      outputNode - pointer to the output node.
*/
void Graph::convince(Node* inputNode, Node* outputNode){
    if(inputNode->getMalice()==1 || outputNode->getMalice()==1){ //if any node is a bot
        inputNode->setState(false);
        outputNode->setState(false);
        this->stat0++;
        this->stat1--;
    }
    else if(inputNode->getMalice()==0 && inputNode->getMalice()==0){ //if no bot
        outputNode->changeState();
        if(outputNode->getState()){ //In this case, a person converts from false to true
            this->stat1++;
            this->stat0--;
        }
        else{ //In this case,a person converts from true to false
            this->stat0++;
            this->stat1--;
        }
    }
}

/**
 * This function is responsible for the deletion of an old edge and the creation of a new edge which happened as a result of interation between 2 nodes of 2 different states.
 * Input Parameters :   adderNode - pointer to the node who will switch friends. Has active discordant edge to deleterNode.
 *                      deletertNode - pointer to the node who will loose adderNode as a friend (inactivation).
*/
void Graph::rewire(Node* adderNode, Node* deleterNode){
    //TODO: Rework this part
    Node* inactiveNode=nullptr; //this person will gain adderNode as a friend
    if(deleterNode->getMalice()==1 && adderNode->getState()) //might be useless
        inactiveNode=adderNode->getRandomInactiveZeroStateEdge();
    else if(adderNode->getMalice()==0 && deleterNode->getMalice()==1)
        inactiveNode=adderNode->getRandomInactiveEdge();
    if(inactiveNode==nullptr)
        return;
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
        std::cout<<"Negative node requested"<<std::endl;
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
 * Iterates through all the nodes and gives the number of discordant edges except for nodes which have no inactive edges.
 * Ignores all edges connected to a malicious user.
 * Retrun : Long Integer containing the count of discordant edges
*/
//TODO: I think this function can be made faster by using this->edges instead of neighbours
long Graph::getActiveDiscordantEdgeCount() const{
    long count=0;
    for(Node* node: nodeList){
        if(!node->hasInactiveEdge() || node->getMalice()!=0) //we don't count inactive discordant edges and we don't count any discordant edge to a bot
            continue;
        for(const auto& pair:node->getNeighbours()){
            if(!pair.second.second) //Not counting inactive edges
                continue;
            Node* neighbour=pair.second.first;
            if(node->getState()!=neighbour->getState() && neighbour->getMalice()==0 && node->getMalice()==0)
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
        if(pair.second.first->getState()!=node->getState())
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
        if(!pair.second.second) //check if edge is active
            continue;
        if(pair.second.first->getState()!=node->getState()) //check if edge is discordant
            candidates.push_back(pair.second.first);
    }
    if(candidates.size()==0){
        //std::cout<<"Active discordant edge retrival failed"<<std::endl;
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

void Graph::saveGraphConfig(bool start, bool bots){
    std::string graphFileName=this->outputFileName.substr(20, this->outputFileName.length()-24);
    
    if(start){
        if(bots)
            graphFileName="../output/NetworkConfig/"+graphFileName+"_start_bots.txt";
        else
            graphFileName="../output/NetworkConfig/"+graphFileName+"_start_nobots.txt";
    }
    else{
        if(bots)
            graphFileName="../output/NetworkConfig/"+graphFileName+"_end_bots.txt";
        else
            graphFileName="../output/NetworkConfig/"+graphFileName+"_end_nobots.txt";
    }

    std::ofstream graphFile;
    graphFile.open(graphFileName);
    for(Node* node: this->nodeList){
        if(!bots && node->getMalice()>0)
            continue;
        std::string line=std::to_string(node->getId())+" ("+std::to_string(node->getState())+") -> ";
        for (const auto& [key, value] : node->getNeighbours()) {
            Node* nodePtr = value.first;  // Get Node*
            bool flag = value.second;   
            if(flag){
                if(!bots && nodePtr->getMalice()>0)
                    continue;
                line+=std::to_string(nodePtr->getId())+" ";
            }
        }
        graphFile<<line<<std::endl;
    }
    graphFile.close();
    std::cout<<"Recorded Graph Config = "<<graphFileName<<std::endl;
}

// void Graph::rewire(Node* nodeA, Node* nodeB){
//     //TODO: Rework this part
//     Node* inactiveNode=nullptr; //this is the new girlfriend
//     Node* switcherNode; //this guys breaks up and finds someone else
//     Node* victimNode; //gets heart broken
//     if(nodeA->getMalice()>0){
//         switcherNode=nodeB;
//         victimNode=nodeA;
//     }
//     else if(nodeB->getMalice()>0){
//         switcherNode=nodeA;
//         victimNode=nodeB;
//     }
//     else{ 
//         double r=this->getRandomNumber();
//         if(r>0.5){
//             switcherNode=nodeA;
//             victimNode=nodeB;
//         }
//         else{
//             switcherNode=nodeB;
//             victimNode=nodeA;
//         } 
//     }
//     inactiveNode=switcherNode->getRandomInactiveEdge();
//     switcherNode->activateEdge(inactiveNode);
//     switcherNode->inactivateEdge(victimNode);
//     victimNode->inactivateEdge(switcherNode);
//     inactiveNode->activateEdge(switcherNode);
// }