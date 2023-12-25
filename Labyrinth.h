#ifndef LABYRINTH_LABYRINTH_H
#define LABYRINTH_LABYRINTH_H

#include "WeightedGraph.h" // Include the declaration of the WeightedGraph class
#include <iostream> // Input/output stream functionality
#include <vector> // Dynamic array functionality using vectors
#include <list> // Linked list functionality
#include <set> // Set container functionality

// Class representing a labyrinth, derived from WeightedGraph
class Labyrinth : public WeightedGraph {
public:
    // Dijkstra's algorithm for finding the shortest path in the labyrinth
    static std::vector<WeightedGraph::Vertex> Dijkstra(Graph &graph, int start, int end);

    // A* algorithm for finding the optimal path in the labyrinth
    static std::vector<Vertex> AStar(Graph& graph, int start, int end, int rows, int numberV);

    // Calculate the Euclidean distance between two points
    static double distance(int xa, int ya, int xb, int yb);

    // Check if the prisoner can move to a specified vertex
    static bool canMove(int vertex_num, const std::vector<Vertex>& vertexList, int numberV);

    // Determine the movement direction for each turn in the labyrinth
    static std::vector<char> MovementDirectionForEachTurn(Graph& graph, int start, int end, int rows, int numberV);

    // Move the prisoner based on the given direction
    static bool movePrisoner(char possibleMovementDirectionForThisTurn, std::vector<Vertex> &vertexList, int lines, int rows);

    // Check if the prisoner reaches the exit and wins the game
    static bool winMove(int start, const std::vector<Vertex>& vertexList, int lines, int rows);

    // Simulate the propagation of fire around a given vertex
    static bool firePropagationAround(int vertexNum, std::vector<Vertex>& vertexList, int lines, int rows);

    // Run an instance of the labyrinth game and return the result (Y for win, N for loss)
    static char runInstance(Graph graph, int start, int end, int lines, int rows);
};

#endif //LABYRINTH_LABYRINTH_H
