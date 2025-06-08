# Opinion Manipulation in Static Complex Networks using Voter Model

This project implements a modified Voter Model to study **opinion dynamics** in a **static network** influenced by bots spreading misinformation (i.e., a "false" opinion). The network evolves through node interactions along **active discordant edges**.

##  Model Overview

- **Node States:** Each node holds a boolean state — `0` (true) or `1` (false).
- **Edge Types:**
  - **Active/Inactive:** Only active edges allow interactions.
  - **Discordant Edge:** Connects nodes with different states.
  - **Harmonious Edge:** Connects nodes with the same state.

- **Static Network:** Node and edge topology remains fixed. Only **edge activity status** may change.
- **Subgraph:** Each simulation starts with a subgraph by randomly deactivating edges.

## Important Parameters

- **Rewiring Rate(β):** Probability of the node using rewiring technique during interaction. Its complement is the convincing rate.
- **Minority Opinion Density(ρ):** No. of real nodes with state `0` / No. of real nodes.
- **Starting Opinion Density:** The minority opinion density at the start of the simulation.
- **Active Discordant Edge Count (l<sub>01</sub>):** No. of active discorant edges between Real Nodes only.
- **Bot Density(α):** % of bots to be added to the network.

##  Interaction Dynamics

Each epoch (time step):
1. A random node is selected.
2. A random neighbor is selected through an **active discordant edge**.
3. One of two actions occurs:
   - **Convince:** One node adopts the other's state.
   - **Rewire:** The discordant edge is inactivated; an inactive harmonious edge is activated.

> Interactions only occur on **active discordant edges**. Harmonious or inactive edges are ignored.

##  Bot Behavior

- Bots always hold the **false** opinion (`0`).
- BotRatio is defined as a percentage of the real nodes (e.g. 10% of 100 real nodes = 10 bots, total bots in network = 110).
- Bots do not rewire. They only attempt to convince real nodes.
- Real nodes cannot convince bots.
- Bot-to-bot edges are disallowed.
- Bot edge densities can be studied as well. Currently, its 2 times the average edge density in the network.

##  Simulation End Criteria

The simulation continues until one of the following occurs:
- No **active discordant edges** remain.
- The network reaches a **steady state**.

##  Research Significance

This model investigates how **malicious agents (bots)** influence consensus in a network, and how **rewiring mechanisms** can resist the spread of false information. It allows experimentation with:
- Bot ratios
- Rewiring strategies

##  Project Structure

src/  
├── Node.cpp / Node.h  
├── Edge.cpp / Edge.h  
├── Graph.cpp / Graph.h  
├── botMain.cpp  
src2/  
├── Node.cpp / Node.h  
├── Edge.cpp / Edge.h  
├── Graph.cpp / Graph.h  
├── Bots.cpp  
├── NoBot.cpp  
├── subprocesses.py  
test/  
├── test_graph.cpp  
├── test_graph.cpp  
├── test_graph.cpp  
visualizations/  
&emsp;outputs/  
&emsp;src/  
&emsp;├── ConnectedComponents.ipynb  
&emsp;├── LinePlot.ipynb  
&emsp;├── MinorityOpinionDensityVsDiscordantEdgeCount.ipynb  
&emsp;├── MinorityOpinionVsRewiring.ipynb  
&emsp;├── PhasePlot.ipynb  
input/  
output/  
Makefile  
README.md  

The **src** folder contains an older version of the code which uses different data strcutures to achieve similar results. However, I personally felt their time complexities were a bit of an issue so I created a new folder called **src2** which contains the latest code.  
When performing executions, please adjust the parameters in Bots.cpp and NoBot.cpp and then just run the subprocesses.py file.

The **visualizations** folder contains the outputs folder where the images of the visualizations are stored while the src folder contains the python notebooks to create said visualizations. The visualizations utilize the data stored in the outermost **input** and **output** folders. 


## ⚙️ Getting Started

1. Clone the repository:
    ```bash
   git clone https://github.com/sagarswap/OpinionManipulationStaticNetworks.git
   cd opinion-voter-model
    ```

2. Run a simulation directly from Bots.cpp or NoBot.cpp
    ```bash
    g++ NoBot.cpp Node.cpp Edge.cpp Graph.cpp -o output
    ./output
    ``` 
