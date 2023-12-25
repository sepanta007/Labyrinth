#ifndef LABYRINTH_WEIGHTEDGRAPH_H
#define LABYRINTH_WEIGHTEDGRAPH_H

#include <iostream> // Input/output stream functionality
#include <vector> // Dynamic array functionality using vectors
#include <list> // Linked list functionality

// Class representing a weighted graph
class WeightedGraph {
public:
    // Inner class to represent a weighted edge
    class Edge {
    public:
        int source; // Source vertex
        int destination; // Destination vertex
        double weight; // Weight of the edge

        // Edge constructor
        Edge(int source, int destination, double weight);
    };

    // Inner class to represent a vertex in the weighted graph
    class Vertex {
    public:
        double indivTime{}; // Individual time of the vertex
        double timeFromSource{}; // Time from the source to this vertex
        double heuristic; // Heuristic of the vertex
        Vertex* prev; // Previous vertex in the optimal path
        std::list<Edge> adjacencyList; // List of edges adjacent to the vertex
        int num; // Vertex number
        char label; // Vertex label
        int i; // Coordinate i of the vertex
        int j; // Coordinate j of the vertex

        // Vertex constructor
        Vertex(int num, char label, int i, int j);

        // Method to add a neighbor (edge) to the vertex
        void addNeighbor(const Edge& e);
    };

    // Main class representing the weighted graph
    class Graph {
    public:
        std::vector<Vertex> vertexList; // List of vertices in the graph
        int num_v = 0; // Number of vertices in the graph

        // Graph constructor
        Graph();

        // Method to add a vertex to the graph
        void addVertex(char label, double indivTime, int i, int j);

        // Method to add a weighted edge between two vertices
        void addEdge(int source, int destination, double weight);
    };
};

#endif //LABYRINTH_WEIGHTEDGRAPH_H
