#include <iostream>
#include <vector>
#include <random>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <cmath>
using namespace std;

/**
 * This is the main program file for Applied Data Science 1. This is made by Swapnil Sagar (ss3854@g.rit.edu) 
 * The file ccontains 2 classes: Node and ComplexNetwork. 
 * A main() exists outside both classes and is used to run the program.
*/


/**
 * This is the node class.
 * The member variables are:
 *      id - Int variable containing the id number of the node. The id can have values ranging from 0 to total Number of Nodes and is automatically assigned by the program while creating the node, stating from 0 and incrementing by 1 every time.
 *      size - An integer variable which will hold the size of the neighbours' array so that it can be dynamically allocated later.
 *      state - A boolean variable which contains the state (0/1) of the node. Randomly assigned while creating the node and can change when interacting with a node which has a different state.
 *      neighbours - An integer array containing the id numbers of all of its neighbours
*/
class Node{
    public:

    int id, size;
    bool state;
    std::vector<int> neighbours;

    Node(int identity, bool status){
        this->id = identity;
        this->state = status;
        this->size=0;
    }

    bool getState(){
        return this->state;
    }

/**
 * This function toggles the state of the node.
*/
    void changeState(){
        this->state = !this->getState();
    }

    int getId(){
        return this->id;
    }

    int getNeighbourCount(){
        return size;
    }

    void setState(bool newState){
        this->state=newState;
    }

/**
 * The next 2 functions activate/inactivate the edge. This is done by making the neighbour a negative value.
 * Inorder to activate the edge, we make the neighbour a positive value again.
 * Input parameter: a positive int variable which contains the neighbour ID we want to actiave/deactivate
*/
    void inactivateEdge(int neighbour){
        bool found=false;
        for(int i=0;i<this->neighbours.size();i++){
            if(neighbours[i] == neighbour && neighbours[i]>=0){
                neighbours[i]*=-1;
                found=true;
                break;
            }
            else if(neighbours[i] == -1*neighbour){
                //cout<<"Already Inactive"<<endl;
                found=true;
                break;
            }
        }
        if(!found)
            cout<<"Edge not found"<<endl;
    }

    void activateEdge(int neighbour){
        bool found=false;
        for(int i=0;i<this->neighbours.size();i++){
            if(neighbours[i] == -1*neighbour && neighbours[i]<0){
                neighbours[i]*=-1;
                found=true;
                break;
            }
            else if(neighbours[i] == neighbour){
                cout<<"Already Active"<<endl;
                found=true;
                break;
            }
        }
        if(!found)
            cout<<"Edge not found"<<endl;
    }

/**
 * This function adds a different node's id to the node's list of neighbours. 
 * Input parameter: an integer variable containing the id of the neighbouring node to be added.
 * Return Type: 0 if sucess, -1 if fail
*/
    bool addNeighbour(int newneigh){
        if(isNeighbour(newneigh)==-1) {
            this->neighbours.push_back(newneigh);
            size++;
            return true;
        }
        else{
            //cout<<this->getId()<<" and "<<newneigh<<" are already neighbours"<<endl;
            return false;
        }
    }

/**
 * This function prints all neighbours of a node. Primarily used for debugging various parts of the program.
 * Input parameter: an integer variable denoting the max number of neighbours to be printed, incase a node has a lot of neighbours
*/
    void printAllNeighbours(int lim){
        for(int i=0; i<this->size && i<lim; i++)
            cout<<this->neighbours[i]<<", ";
        cout<<endl;
    }
    void printAllNeighbours(){
        for(int i=0; i<this->size; i++)
            cout<<this->neighbours[i]<<", ";
        cout<<endl;
    }

/**
 * This function prints all active neighbours of a node. Primarily used for debugging various parts of the program.
 * Input parameter: an integer variable denoting the max number of neighbours to be printed, incase a node has a lot of neighbours
*/
    void printAllActiveNeighbours(int lim){
        for(int i=0; i<this->size && i<lim; i++)
            if(this->neighbours[i]>=0)
                cout<<this->neighbours[i]<<", ";
        cout<<endl;
    }
    void printAllActiveNeighbours(){
        for(int i=0; i<this->size; i++)
            if(this->neighbours[i]>=0)
                cout<<this->neighbours[i]<<", ";
        cout<<endl;
    }

/**
 * This function iterates through the neighbours vector to check if a given node id is present in the list.
 * The first method checks regardless if the neighbour edge is active or not. The following method checks for active edge only.
 * Input parameter: A positive integer variable containing the id of the node we want to check along with a char variable 
 * Return values: Returns the location of the id being stored in the vector if it is present if option='i'; 
 *                Returns identity if option=Returns -1 if the node is not a neighbour
*/
    //returns -1 if identity is not a neighbour, else returns index number 
    int isNeighbour(int identity){
        for(int i=0; i<this->size; i++)
            if(neighbours[i]==identity || neighbours[i]==-1*identity)
                return i;
        return -1;
    }

    int isActiveNeighbour(int identity){
        for(int i=0; i<this->size; i++)
            if(neighbours[i]==identity && neighbours[i]>0)
                return i;
        return -1;
    }

/**
 * This function is meant for debugging purposes. Returns True if neighbour and false if not.
*/
    bool neighbourCheck(int identity){
        for(int i=0; i<this->size; i++)
            if(neighbours[i]==identity || neighbours[i]==-1*identity)
                return true;
        return false;
    }

    bool hasInactiveEdge(){
        for(int n:neighbours)
            if(n<0)
                return true;
        return false;
    }

/**
 * retuns the ID of a random in-active edge among the neighbours of this node.
 * Return Value: Returns the positive id of the in-active edge. Returns -1 if there are no inactvie edges.
*/
    int getInactiveEdgeID(){
        std::vector<int> candidates;
        for(int n: neighbours){
            if(n<0)
                candidates.push_back(abs(n));
        }
        if(candidates.size()==0){
            cout<<"No Inactive Edge found";
            return -1;
        }
        else if(candidates.size()==1)
            return candidates[0];
        else{
            int rand=getRandomNumber(candidates.size()-1);
            return candidates[rand];
        }
    }

    int getRandomNumber(int limit){
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<int> dist(0, limit); 
        return dist(rng);
    }
};






class Edge{
    Node* nodeA;
    Node* nodeB;
    bool active;
    public:
    int nodeAID, nodeBID;
    
    Edge(Node* A, Node* B){
        nodeA = A;
        nodeB = B;
        nodeAID = A->id;
        nodeBID = B->id;
        active = true;
    }

    bool getStatus(){
        return active;
    }

    void activateEdge(){
        if(active){
            cout<<"Edge is already active!"<<endl;
            return;
        }
        active=true;
    }

    void inactivateEdge(){
        if(!active){
            //cout<<"Edge is already inactive!"<<endl;
            return;
        }
        active=false;
    }

    void printNodes(){
        cout<<"The nodes are "<<nodeAID<<" and "<<nodeBID<<" with status = "<<active<<endl;
    }

    Node* getNodeA(){
        if(nodeA==nullptr){
            cout<<"Node A reference is null"<<endl;
            return nullptr;
        }
        else
            return nodeA;
    }

    Node* getNodeB(){
        if(nodeB==nullptr){
            cout<<"Node B reference is null"<<endl;
            return nullptr;
        }
        else
            return nodeB;
    }
};







/**
 * This is the ComplexNetwork class. It creates several instances of the Node class and stores their locations by reference so that we can access every node. 
 * This class is also responsible for the interaction between the nodes in the Complex System.
 * The memeber variables are:
 *      nodeCount - An integer variable which keeps a count of the total Number of nodes in the network.
 *      edgeCount - An integer variable which keeps a count of the total number of edges in the network.
 *      stat0 - An integer variable which keeps a count of the number of nodes with state = 0/false
 *      stat1 - An integer variable which keeps a count of the number of nodes with state = 1/true
 *      nodeList - A vector of Node* type which contains pointers to every Node so that we can call them by reference.
 *      filename - A string object which contains the relative location of the data file which will be used to generate the network.
*/
class ComplexNetwork{
    public:
    //node status as false is related to stat0 while true is related to stat1
    long nodeCount, edgeCount, stat0, stat1;
    int epochLimit, stepCount;
    double rewiringProbability, relativeSize, startingRatio;
    std::vector<Node*> nodeList;
    std::vector<Edge*> edgeList;
    std::string inputFileName, outputFileName;
    long rew=0, con=0;
    bool complexContagion;

    ComplexNetwork(std::string infname, double rewire, bool contagion, double ratio){
        cout<<"Constructor reached"<<endl;
        int n=rewire*100.0;
        startingRatio=ratio;
        std::string rat=getSubFolderName(ratio);
        std::string cont;

        if(contagion)
            cont="ComplexContagion/";
        else
            cont="SimpleContagion/";
        if(infname.substr(0, 5)=="RealW"){
            std::string fname=infname.substr(10);
            this->inputFileName="../data/input/"+infname+".txt";
            this->outputFileName="../data/output/"+infname+"/"+cont+""+rat+fname+"_r"+std::to_string(n)+"_"+std::to_string(getRandomNumber(10000))+".txt";
        }
        else if(infname.substr(0, 5)=="Watts"){
            std::string pVal=infname.substr(infname.size()-2, 2)+"/";
            this->inputFileName="../data/input/WattsStrogatz/"+infname+".txt";
            this->outputFileName="../data/output/WattsStrogatz/"+cont+rat+pVal+"ws_r"+std::to_string(n)+"_"+std::to_string(getRandomNumber(10000))+".txt";
        }
        else if(infname.substr(0, 5)=="Erdos"){

        }
        stat0=0;
        stat1=0;
        epochLimit=10000000;
        stepCount=200;
        rewiringProbability=rewire; 
        relativeSize=0.5;
        complexContagion=contagion;
    }

/**
 * This function reads the text file which was defined while creating the object of ComplexNetwork. 
 * The first line of the file contains the nodeCount and the edgeCount.
 * It calls generateNetwork() which creates the network and sets up other parameters.
 * This function then connects the entire network by defining the edges between the nodes.
*/
    void loadData(){
        cout<<"Data Load Start"<<endl;
        fstream file;
        file.open(this->inputFileName, ios::in);
        string tp;
        int cc=0;
        while(getline(file, tp)){
            std::istringstream is(tp);
            int inputNode, outputNode;
            is>>inputNode;
            is>>outputNode;
            if(cc==0){
                cout<<"Start Node Generation"<<endl;
                generateNetwork(inputNode, outputNode);
                cc++;
            }
            else{
                inputNode++;//this is done because node ids begin with 1 but uses a 0-indexed array
                outputNode++;
                if(inputNode==outputNode)
                    continue; //prevents edges into self
                Node* node1=getNode(inputNode);
                Node* node2=getNode(outputNode);
                Edge* edge=new Edge(node1, node2);
                edgeList.push_back(edge);
                node1->addNeighbour(outputNode);
                node2->addNeighbour(inputNode);
            }        
        }
        cout<<"Data Loaded"<<endl;
        file.close();
        generateSubNetwork();
        cout<<"Nodes = "<<this->nodeCount<<"\tEdges = "<<this->edgeCount<<endl;
    }

/**
 * This function returns the reference to a node.
 * Input Parameter - An integer variable containing the id of the node to be called. It should range between 1 and nodeCount (inclusive)
 * Return Value - A pointer to the node if it exists.
*/
    Node* getNode(int identity){
        if(identity<=0){
            cout<<"Negative node requested"<<endl;
            //identity*=-1;
        }
        if(nodeCount>=identity)
            return nodeList[identity-1];
        cout<<"Node with id = "<<identity<<" not found"<<endl;
        return nullptr;
    }

/**
 * This function prints all the edges in the network by iterating through the nodeList and calling printAllNeighbours() for every node.
 * Input Parameter - An integer variable denoting the max number of neighbours to be printed, incase a node has a lot of neighbours
*/
    void printAllEdges(int lim){
        for(int i=0; i<nodeCount; i++){
            Node* node=nodeList[i];
            cout<<node->getId()<<" -> ";
            node->printAllNeighbours(lim);
        }
    }

/**
 * This function returns a string containing the number of nodes in each state at that particular time in the network, its opinion density, number of discordant edges and epoch number
*/
    std::string getSummary(int epoch, long discEdge){
        std::ostringstream oss;
        oss<<(epoch+1)<<" "<<stat0<<" "<<(stat0/(stat1*1.0+stat0*1.0))<<" "<<discEdge;
        return oss.str();
    }

/**
 * This function initialises the values of nodeCount, edgeCount, stat0 and stat1, creates every Node and assigns a random state to each one of them.
 * Input Parameters :   nodeC - An integer variable containing the number of nodes in the network.
 *                      edgeC - An integer variable containing the number of edges in the network.
*/
    void generateNetwork(int nodeC, int edgeC){
        this->nodeCount=nodeC;
        this->edgeCount=edgeC;
        for(int i=1; i<=nodeC; i++){
            double randState=getRandomNumber();
            Node* newNode;
            if(randState<this->startingRatio){
                stat0++;
                newNode=new Node(i, false);
            }
            else{
                stat1++;
                newNode=new Node(i, true);
            }
            
            this->nodeList.push_back(newNode);
        }
        cout<<"Nodes Generated"<<endl;
    }

    void generateSubNetwork(){
        for(Edge* edge: edgeList){
            double rando=this->getRandomNumber();
            if(relativeSize<rando){
                inActivateEdge(edge);
            }
        }
        cout<<"Subnetwork Generated"<<endl;
    }

/**
 * This function starts the simulation by calling the interact() as well as regularly printing the results after every stepCount loop.
 * Input Parameters :   epochLimit - An integer variable which defines the duration of execution of the simulation.
 *                      stepCount - An integer variable which defines the number of steps in each epoch. The total duration of the simulation = epochLimit*stepCount.
 *                      rewiringProbability - The frequency with which nodes ineract with each other.
*/
    void beginSimulation(){
        long discEdge=this->getActiveDiscordantEdgeCount();
        bool altEdgeSelectionAlgo=false;
        cout<<getSummary(-1, discEdge)<<endl;
        ofstream outputFile;
        outputFile.open(this->outputFileName);
        outputFile<<"Epoch Pop Frac DiscEdge"<<endl;
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
                    cout<<summary<<endl;
                    outputFile << summary <<endl;
                    this->recountStates();
                    cout<<"Simulation Completed!"<<endl;
                    outputFile.close();
                    return;
                }
            }
            if(epoch%10==0){
                cout<<"Epoch No. "<<epoch<<endl;
            }
            discEdge=this->getActiveDiscordantEdgeCount();
            if(discEdge==0){
                std::string summary=getSummary(epoch, discEdge);
                cout<<summary<<endl;
                outputFile << summary <<endl;
                this->recountStates();
                cout<<"Simulation Completed!"<<endl;
                outputFile.close();
                return;
            }
            std::string summary=getSummary(epoch, discEdge);
            cout<<summary<<endl;
            outputFile << summary <<endl;

            if(discEdge<100 && !altEdgeSelectionAlgo){
                altEdgeSelectionAlgo=true;
                cout<<"Switching Algorithms"<<endl;
            }
        }
        outputFile.close();
    }

/**
 * This function is responsible for the interactions between the nodes, which are further controlled by convince() and rewire().
 * It iterates through every edge and uses random number generation to decide on its interaction
 * Input Parameter - An integer variable between 0 and 0.1 which controls how often an interaction occurs. 
*/
    bool interact(){
        bool ideal=false;
        double rando=this->getRandomNumber();
        Node* node;
        int tries=0;
        do{
            rando=this->getRandomNumber();
            node=this->getRandomNode();
            if(rando<=rewiringProbability){
                if(node->hasInactiveEdge() && this->hasActiveDiscordantEdge(node))
                    ideal=true;
            }
            else{
                if(this->hasActiveDiscordantEdge(node))
                    ideal=true;
            }
            tries++;
        }while(!ideal && tries<10000);
        
        if(tries>=1000){
            cout<<"Tries is greater with a value of "<<tries<<endl;
            return true;
        }

        if(rando<=rewiringProbability){
            Node* neighbour=this->getActiveDiscordantEdge(node);
            this->rewire(node, neighbour);
        }
        else{ 
            if(!this->complexContagion){
                Node* neighbour=this->getActiveDiscordantEdge(node);
                this->convince(node, neighbour);
            }
            else
                this->convinceComplexContagion(node);
        }
        return false;
    }

    bool interactAlt(){
        std::vector<Node*> roster;
        double rando=this->getRandomNumber();
        for(Node* node: nodeList){
            if(rando<=rewiringProbability){
                if(node->hasInactiveEdge() && this->hasActiveDiscordantEdge(node))
                    roster.push_back(node);
            }
            else{
                if(this->hasActiveDiscordantEdge(node))
                    roster.push_back(node);
            }
        }
        if(roster.size()<=1 || this->getActiveDiscordantEdgeCount()<=1){
            this->recountStates();
            return true;
        }
        int rand=this->getRandomNumber(roster.size()-1);
        //cout<<"random "<<rand<<" "<<edges.size()<<endl;
        Node* node1=roster[rand];
        if(rando<=rewiringProbability){
            Node* node2=this->getActiveDiscordantEdge(node1);
            this->rewire(node1, node2);
        }
        else{ 
            if(!this->complexContagion){
                Node* node2=this->getActiveDiscordantEdge(node1);
                this->convince(node1, node2);
            }
            else
                this->convinceComplexContagion(node1);
        }

        return false;
    }

/**
 * This function is used to change the state of the neighbour with which our node interacted with and now hold the same state.
 * Input Parameters :   inputNode - pointer to the input node.
 *                      outputNode - pointer to the output node.
*/
    void convince(Node* inputNode, Node* outputNode){
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

    void convinceComplexContagion(Node* node){
        int op0=0, op1=0;
        for(int neighbourID: node->neighbours){
            if(neighbourID<0)
                continue;
            Node* neighbour=this->getNode(neighbourID);
            if(neighbour->getState())
                op1++;
            else
                op0++;
        }
        double switchToTrue=op1/(op1+op0);
        double rando=this->getRandomNumber();
        if(rando<=switchToTrue){
            if(!node->getState()){
                node->changeState();
                this->stat1++;
                this->stat0--;
            }
        }
        else{
            if(node->getState()){
                node->changeState();
                this->stat0++;
                this->stat1--;
            }
        }
    }

/**
 * This function is responsible for the deletion of an old edge and the creation of a new edge which happened as a result of interation between 2 nodes of 2 different states.
 * Input Parameters :   inputNode - pointer to the input node.
 *                      outputNode - pointer to the output node.
*/
    void rewire(Node* adderNode, Node* deleterNode){
        Node* inactiveNode=this->getNode(adderNode->getInactiveEdgeID());
        adderNode->activateEdge(inactiveNode->getId());
        inactiveNode->activateEdge(adderNode->getId());
        adderNode->inactivateEdge(deleterNode->getId());
        deleterNode->inactivateEdge(adderNode->getId());
    }

    void inActivateEdge(Edge* edge){
        edge->inactivateEdge();
        Node* nodeA=edge->getNodeA();
        Node* nodeB=edge->getNodeB();
        nodeA->inactivateEdge(edge->nodeBID);
        nodeB->inactivateEdge(edge->nodeAID);
    }

    void activateEdge(Edge* edge){
        edge->activateEdge();
        Node* nodeA=edge->getNodeA();
        Node* nodeB=edge->getNodeB();
        nodeA->activateEdge(edge->nodeBID);
        nodeB->activateEdge(edge->nodeAID);
    }

/**
 * Iterates through all the nodes and gives the number of discordant edges except for nodes which have no inactive edges
 * Retrun : Long Integer containing the count of discordant edges
*/
    long getActiveDiscordantEdgeCount(){
        long count=0;
        for(Node* node: nodeList){
            if(!node->hasInactiveEdge())
                continue;
            for(int id:node->neighbours){
                if(id<=0)
                    continue;
                Node* neighbour=this->getNode(id);
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
    bool hasActiveDiscordantEdge(Node* node){
        for(int neigh: node->neighbours){
            if(neigh<=0)
                continue;
            Node* n=this->getNode(neigh);
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
    Node* getActiveDiscordantEdge(Node* node){
        std::vector<Node*> candidates;
        for(int neigh: node->neighbours){
            if(neigh<0)
                continue;
            Node* n=this->getNode(neigh);
            if(n->getState()!=node->getState())
                candidates.push_back(n);
        }
        if(candidates.size()==0){
            cout<<"Active discordant edge retrival failed"<<endl;
            return nullptr;
        }
        else if(candidates.size()==1)
            return candidates[0];
        else{
            int rand=this->getRandomNumber(candidates.size()-1);
            return candidates[rand];
        }
    }

/**
 * This function generates a random number between 0 and limit, and returns it to the caller
 * Input Parameters : limit - Integer variable which will contain the upper limit of random number generation.
 * Return Value : Integer variable of a random number.
*/
    int getRandomNumber(int limit){
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<int> dist(0, limit); 
        return dist(rng);
    }

    double getRandomNumber(){
        std::random_device rd;
        std::uniform_real_distribution<double> dist(0.0, 1.0);
        return dist(rd);
    }

    Node* getRandomNode(){
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dist(1, this->nodeCount);
        Node* node=this->nodeList[dist(gen)-1];
        if(node==nullptr){
            cout<<"getRandomNode() Error "<<dist(gen)-1<<endl;
            return nullptr;
        }
        return node;
    }

    void checkInconsitentNeighbours(){
        int difference=0;
        for(Node* node: nodeList){
            for(int neigh: node->neighbours){
                if(neigh<0)
                    neigh*=-1;
                Node* neighbour=this->getNode(neigh);
                if(node->neighbourCheck(neighbour->getId()) != neighbour->neighbourCheck(node->getId()))
                    difference++;
            }
        }
        cout<<"Difference = "<<difference<<endl;
    }

/*This function verifies if our counts of the node states is correct. It will print an error message incase it isn't*/
    void recountStates(){
        int s1=0, s2=0;
        for(Node* node: nodeList){
            if(node->getState()==0)
                s1++;
            else
                s2++;
        }
        if(s1!=this->stat0 || s2!=this->stat1)
            cout<<"Counting Mismatch"<<endl;
    }

/**
 * This function is to check if in the network, all active edges have '+'ve values in edgeList and all inactive edges have '-' values in edgeList
*/
    void checkEdges(){
        bool fail=false;
        for(Edge* edge:edgeList){
            Node* nodeA=edge->getNodeA();
            Node* nodeB=edge->getNodeB();
            if(edge->getStatus()){
                if(nodeA->neighbourCheck(edge->nodeBID) && nodeB->neighbourCheck(edge->nodeAID)){
                    cout<<"Edge Check failed at ";
                    edge->printNodes();
                    fail=true;
                }
            }
            else{
                if(nodeA->neighbourCheck(edge->nodeBID) || nodeB->neighbourCheck(edge->nodeAID)){
                    cout<<"Edge Check failed at ";
                    edge->printNodes();
                    fail=true;
                }
            }
        }
    }

    //Code for debugging purposes
    void checkInactivity(Node* n1, Node* n2){
        int v1=0, v2=0;
        for(int x: n1->neighbours){
            if(abs(x)==n2->getId())
                v1=x;
        }
        for(int x: n2->neighbours){
            if(abs(x)==n1->getId())
                v2=x;
        }

        if(v1==0 || v2==0){
            cout<<"Neighbour actually not present"<<endl;
            cout<<n1->getId()<<" -> ";
            n1->printAllNeighbours(100);
            cout<<n2->getId()<<" -> ";
            n2->printAllNeighbours(100);
        }
        else if((v1>0 && v2<0) || (v1<0 && v2>0))
            cout<<"Activations are of different signs"<<endl;
    }

/**
 * Prints all the edges along with their status. Also prints  the number of active and inactive edges. For debugging pruposes.
*/
    void printAllEdges(){
        int count=0, act=0, ina=0;
        for(Edge* edge: edgeList){
            edge->printNodes();
            count++;
            if(edge->getStatus())
                act++;
            else
                ina++;
        }
        cout<<count<<" Edges in the Network, "<<act<<" are active and "<<ina<<" are inactive."<<endl;
    }

    std::string getSubFolderName(double ratio){
        switch((int)(ratio*10)) {
            case 1:
                return "01/";
            case 2:
                return "02/";
            case 3:
                return "03/";
            case 4:
                return "04/";
            case 5:
                return "05/";
        }

        return "NAHI";
    }
};

int main(){
    //double rewiring[]={0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9};
    double rewiring[]={0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9};
    double start[]={0.1, 0.2, 0.3, 0.4, 0.5, 0.1, 0.2, 0.3, 0.4, 0.5};
    bool contagi[]={false, true};
    int l1=sizeof(rewiring)/sizeof(rewiring[0]);
    int l2=sizeof(start)/sizeof(start[0]);
    string network[]={"WattsStrogatz_N50000_p0_k10", "WattsStrogatz_N50000_p1_k10", "WattsStrogatz_N50000_p2_k10", "WattsStrogatz_N50000_p3_k10", "WattsStrogatz_N50000_p4_k10", "WattsStrogatz_N50000_p5_k10", "WattsStrogatz_N50000_p6_k10", "WattsStrogatz_N50000_p7_k10", "WattsStrogatz_N50000_p8_k10", "WattsStrogatz_N50000_p9_k10"};
    // for(string nw:network){
    //     for(double  r : rewiring){
    //         ComplexNetwork* network=new ComplexNetwork(nw, 100, 200, r, 0.5); //epochs, steps in epoch, rewiring_factor, subgrah_rel_size
    //         network->loadData();
    //         network->beginSimulation();
    //         //network->printAllEdges();
    //         //network->checkDegreeDistribution();
    //         cout<<"Completed for r="<<r<<" and  Network="<<nw<<endl;
    //         delete network;
    //     }
    // }
    int execution=0;
    for(bool c: contagi){
        for(double  r : rewiring){
            for(double st: start){
                ComplexNetwork* network=new ComplexNetwork("RealWorld/facebookArtist",r, c, st); //rewiring_factor, complexContagion, starting opinion ratio
                network->loadData();
                network->beginSimulation();
                execution++;
                //network->printAllEdges();
                //network->checkDegreeDistribution();
                cout<<"Completed for r="<<r<<" Execution = "<<execution<<"/"<<l1*l2*2<<endl;
                //network->printAllEdges(500);
                delete network;
            }
        }
    }
}