# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -I./src

# Source files
SOURCES = src/Node.cpp src/Edge.cpp src/Graph.cpp

# Test files
TESTS = test/test_node.cpp test/test_edge.cpp test/test_graph.cpp

# Output executable for each test
TEST_EXECUTABLES = test/test_node test/test_edge test/test_graph

all: $(TEST_EXECUTABLES)

test/test_node: $(SOURCES) test/test_node.cpp
	$(CXX) $(CXXFLAGS) -o $@ $(SOURCES) test/test_node.cpp

test/test_edge: $(SOURCES) test/test_edge.cpp
	$(CXX) $(CXXFLAGS) -o $@ $(SOURCES) test/test_edge.cpp

test/test_graph: $(SOURCES) test/test_graph.cpp
	$(CXX) $(CXXFLAGS) -o $@ $(SOURCES) test/test_graph.cpp

clean:
	rm -f $(TEST_EXECUTABLES)

.PHONY: all clean


#To test test_edge, do the following commands:
#make test/test_edge
#./test/test_edge