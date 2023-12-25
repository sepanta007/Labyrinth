#include <limits> // Include the limits header for numeric limits
#include "WeightedGraph.h" // Include the declaration of the WeightedGraph class

// Edge class constructor
WeightedGraph::Edge::Edge(int source, int destination, double weight)
        : source(source), destination(destination), weight(weight) {}

// Vertex class constructor
WeightedGraph::Vertex::Vertex(int num, char label, int i, int j) : num(num), label(label), i(i), j(j) {
    indivTime = std::numeric_limits<double>::infinity();
    timeFromSource = std::numeric_limits<double>::infinity();
    heuristic = -1;
    prev = nullptr;
}

void WeightedGraph::Vertex::addNeighbor(const Edge& e) {
    // Add a neighbor (edge) to the vertex
    adjacencyList.push_front(e);
}

// Graph class constructor
WeightedGraph::Graph::Graph() = default;

void WeightedGraph::Graph::addVertex(char label, double indivTime, int i, int j) {
    // Add a vertex to the graph
    Vertex v(num_v, label, i, j);
    v.indivTime = indivTime;
    vertexList.push_back(v);
    num_v = num_v + 1;
}

void WeightedGraph::Graph::addEdge(int source, int destination, double weight) {
    // Add a weighted edge between two vertices
    Edge edge(source, destination, weight);
    vertexList[source].addNeighbor(edge);
}
