#include "Labyrinth.h" // Include the declaration of the Labyrinth class
#include <algorithm> // Include the algorithm header for std::reverse
#include <cmath> // Include the cmath header for mathematical functions
#include <chrono> // Include the chrono header for time-related functionality
#include <iostream> // Include the iostream header for input/output operations
#include <limits> // Include the limits header for numeric limits
#include <set> // Include the set header for the set container
#include <vector> // Include the vector header for the vector container

std::vector<WeightedGraph::Vertex> Labyrinth::Dijkstra(Graph &graph, int start, int end) {
    // List to store the final path
    std::vector<Vertex> path;

    // Initialization of time from source for all vertices to infinity, except the starting point
    graph.vertexList[start].timeFromSource = 0;
    int number_tries = 0;

    // Set of vertices to visit
    std::set<Vertex*> to_visit;
    for (Vertex& vertex : graph.vertexList)
        to_visit.insert(&vertex);

    // Measure the start time
    auto start_time = std::chrono::high_resolution_clock::now();

    // Main loop of the algorithm
    while (!to_visit.empty()) {
        // Find the vertex with the lowest time from the source
        Vertex* minVertex = nullptr;
        double minimal = std::numeric_limits<double>::infinity();

        for (Vertex* vertex : to_visit) {
            if (vertex->timeFromSource <= minimal) {
                minVertex = vertex;
                minimal = vertex->timeFromSource;
            }
        }

        // Check if the minimum vertex is valid
        if (minVertex == nullptr) {
            return path;
        }

        // Remove the minimum vertex from the set to visit
        to_visit.erase(minVertex);
        number_tries++;

        // Update time from source for adjacent vertices
        for (Edge& edge : minVertex->adjacencyList) {
            int destination = edge.destination;
            // Check the validity of the destination index
            if (destination < 0 || static_cast<size_t>(destination) >= graph.vertexList.size()){
                std::cout << "Invalid destination index in Dijkstra." << std::endl;
                return path;
            }

            Vertex& neighbor = graph.vertexList[destination];
            double newTime = minVertex->timeFromSource + edge.weight;

            // Update if a shorter time is found
            if (newTime < neighbor.timeFromSource) {
                neighbor.timeFromSource = newTime;
                neighbor.prev = minVertex;
            }
        }
    }

    // Measure the end time
    auto end_time = std::chrono::high_resolution_clock::now();

    // Calculate the duration in microseconds
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

    // Output the duration
    std::cout << "Done ! Using Dijkstra ! Elapsed time : " << duration.count() << " microseconds" << std::endl;

    // Reconstruct the path from the end
    Vertex* current = &graph.vertexList[end];
    while (current != nullptr) {
        path.push_back(*current);
        current = current->prev;
    }
    std::reverse(path.begin(), path.end());

    return path;
}

std::vector<WeightedGraph::Vertex> Labyrinth::AStar(Graph& graph, int start, int end, int rows, int numberV) {
    // List to store the final path
    std::vector<Vertex> path;

    // Initialization of time from source for all vertices to infinity, except the starting point
    graph.vertexList[start].timeFromSource = 0;
    int number_tries = 0;

    // Set of vertices to visit
    std::set<int> to_visit;
    for (Vertex& vertex : graph.vertexList)
        to_visit.insert(vertex.num);

    // Calculate heuristics for each vertex
    // It takes into account the coordinates (i % rows, i / rows) of the current vertex
    // And the coordinates (end % rows, end / rows) of the destination point
    int i = 0;
    for (Vertex& vertex : graph.vertexList) {
        vertex.heuristic = distance(i % rows, i / rows, end % rows, end / rows);
        i++;
    }

    // Measure the start time
    auto start_time = std::chrono::high_resolution_clock::now();

    // Main loop of the A* algorithm
    while (to_visit.count(end) > 0) {
        // Find the vertex with the smallest value of timeFromSource + heuristic
        int min_v = 0;
        double timeFromSourceHeuristicMinimal = std::numeric_limits<double>::infinity();

        for (int vertexNum : to_visit) {
            if ((graph.vertexList[vertexNum].timeFromSource + graph.vertexList[vertexNum].heuristic) <= timeFromSourceHeuristicMinimal) {
                min_v = vertexNum;
                timeFromSourceHeuristicMinimal = graph.vertexList[vertexNum].timeFromSource + graph.vertexList[vertexNum].heuristic;
            }
        }

        // Add the current vertex to the path
        path.push_back(graph.vertexList[min_v]);
        to_visit.erase(min_v);
        number_tries += 1;

        // Update timeFromSource values for adjacent vertices
        for (const auto& edge : graph.vertexList[min_v].adjacencyList) {
            int to_try = edge.destination;
            if (to_visit.count(to_try) > 0) {
                bool move = canMove(to_try, graph.vertexList, numberV);

                if (move) { // Prisoner moved
                    double newTime = graph.vertexList[min_v].timeFromSource + edge.weight;
                    if (newTime < graph.vertexList[to_try].timeFromSource) {
                        graph.vertexList[to_try].timeFromSource = newTime;
                        graph.vertexList[to_try].prev = &graph.vertexList[min_v];
                    }
                }
            }
        }
    }

    // Measure the end time
    auto end_time = std::chrono::high_resolution_clock::now();

    // Calculate the duration in microseconds
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

    // Output the duration
    std::cout << "Done ! Using A* ! Elapsed time : " << duration.count() << " microseconds" << std::endl;

    return path;
}

double Labyrinth::distance(int xa, int ya, int xb, int yb) {
    // Calculate the Euclidean distance between two points
    double resultX = std::pow((xb - xa), 2);  // Calculate the squared difference in the x-coordinates
    double resultY = std::pow((yb - ya), 2);  // Calculate the squared difference in the y-coordinates

    return std::sqrt(resultX + resultY);  // Return the square root of the sum of squared differences
}

bool Labyrinth::canMove(int vertex_num, const std::vector<Vertex>& vertexList, int numberV) {
    // Check if the vertex number is valid
    if (vertex_num < numberV) {
        // Retrieve the current vertex from the list of vertices
        const Vertex& currentVertex = vertexList[vertex_num];

        // Check if the label of the vertex is an empty point or a starting point
        return currentVertex.label == '.' || currentVertex.label == 'S';
    } else {
        // The vertex number is invalid; the prisoner cannot move
        return false;
    }
}

std::vector<char> Labyrinth::MovementDirectionForEachTurn(Graph& graph, int start, int end, int rows, int numberV) {
    // Calculate the optimal path using the A* algorithm
    std::vector<Vertex> path = AStar(graph, start, end, rows, numberV);
    Dijkstra(graph, start, end);

    // List to store movement directions
    std::vector<char> directions;

    // Traverse the path to determine movement directions between each pair of vertices
    for (size_t i = 0; i < (path.size() - 1); i++) {
        // Calculate the movement between two adjacent vertices
        int movement = path[i + 1].num - path[i].num;

        // Assign the corresponding direction based on the movement
        if (movement == 1)
            directions.push_back('R'); // Right
        else if (movement == -1)
            directions.push_back('L'); // Left
        else if (movement == rows)
            directions.push_back('U'); // Up
        else if (movement == (-1 * rows))
            directions.push_back('T'); // Down
        else
            return directions; // Stop if the movement is not recognized
    }

    return directions;
}

bool Labyrinth::movePrisoner(char movementDirection, std::vector<Vertex> &vertexList, int lines, int rows) {
    // Move the prisoner based on the given movement direction
    int start = 0;

    // Find the current position of the prisoner
    for (size_t i = 0; i < vertexList.size(); i++) {
        if (vertexList[i].label == 'D')
            start = static_cast<int>(i);
    }

    // Check for victory
    bool victory = winMove(start, vertexList, lines, rows);

    // Return true if victorious
    if (victory)
        return true;
    else {
        vertexList[start].label = 'L'; // Mark the current position as passed

        // Move the prisoner according to the direction
        int newStart = start; // Initialize the new position to the current position

        if (movementDirection == 'U' && (start + rows < static_cast<int>(vertexList.size()))) {
            newStart = start + rows; // Move the prisoner upwards
        } else if (movementDirection == 'T' && (start - rows >= 0)) {
            newStart = start - rows; // Move the prisoner downwards
        } else if (movementDirection == 'L' && (start % rows != 0)) {
            newStart = start - 1; // Move the prisoner to the left
        } else if (movementDirection == 'R' && ((start + 1) % rows != 0)) {
            newStart = start + 1; // Move the prisoner to the right
        }

        // Check if the movement is possible
        if (canMove(newStart, vertexList, lines * rows)) {
            vertexList[newStart].label = 'D'; // Move the prisoner to the new position
            return false; // Successful movement
        } else {
            return false; // Invalid movement
        }
    }
}

bool Labyrinth::winMove(int start, const std::vector<Vertex>& vertexList, int lines, int rows) {
    // Check if the prisoner can move to the cell on the left and if that cell contains the exit
    bool left = vertexList[start].j != 0 && (vertexList[start - 1].label == 'S');
    // Check if the prisoner can move to the cell on the right and if that cell contains the exit
    bool right = vertexList[start].j != (rows - 1) && (vertexList[start + 1].label == 'S');
    // Check if the prisoner can move to the cell above and if that cell contains the exit
    bool top = vertexList[start].i != 0 && vertexList[start - rows].label == 'S';
    // Check if the prisoner can move to the cell below and if that cell contains the exit
    bool bottom = vertexList[start].i != (lines - 1) && vertexList[start + rows].label == 'S';

    // The prisoner wins if they can exit through the top, left, right, or bottom
    return top || left || right || bottom;
}

bool Labyrinth::firePropagationAround(int vertexNum, std::vector<Vertex>& vertexList, int lines, int rows) {
    // Propagate the fire around a given vertex, updating adjacent cells
    bool lose = false;

    // Check and update the cell to the left of the prisoner
    if (vertexNum % rows != 0 && vertexList[vertexNum - 1].label != 'F') {
        vertexList[vertexNum - 1].label = 'A'; // Prevent fire from being in the same cell
        if (vertexList[vertexNum - 1].label == 'S') {
            lose = true;
        }
    }

    // Check and update the cell to the right of the prisoner
    if ((vertexNum + 1) % rows != 0 && vertexList[vertexNum + 1].label != 'F') {
        vertexList[vertexNum + 1].label = 'A';
        if (vertexList[vertexNum + 1].label == 'S') {
            lose = true;
        }
    }

    // Check and update the cell above the prisoner
    if (vertexNum >= rows && vertexList[vertexNum - rows].label != 'F') {
        vertexList[vertexNum - rows].label = 'A';
        if (vertexList[vertexNum - rows].label == 'S') {
            lose = true;
        }
    }

    // Check and update the cell below the prisoner
    if (vertexNum < (lines - 1) * rows && vertexList[vertexNum + rows].label != 'F') {
        vertexList[vertexNum + rows].label = 'A';
        if (vertexList[vertexNum + rows].label == 'S') {
            lose = true;
        }
    }

    // Return true if the fire reaches the prisoner in this turn
    return lose;
}

char Labyrinth::runInstance(Graph graph, int start, int end, int lines, int rows) {
    // Run an instance of the labyrinth game and return the result ('Y' for win, 'N' for lose)
    int turn = 0;

    // Calculate movement directions for each turn
    std::vector<char> directions = MovementDirectionForEachTurn(graph, start, end, rows, lines * rows);

    while (turn < static_cast<int>(directions.size())) {
        // Update fire states on the graph
        for (auto & vertex : graph.vertexList) {
            if (vertex.label == 'A') // Fire movement
                vertex.label = 'F'; // Fire
        }

        // Fire propagation and check for game over
        for (size_t i = 0; i < graph.vertexList.size(); i++) {
            if (graph.vertexList[i].label == 'F') {
                if (firePropagationAround(static_cast<int>(i), graph.vertexList, lines, rows))
                    return 'N'; // Lost
            }
        }

        // Move the prisoner
        if (movePrisoner(directions[turn], graph.vertexList, lines, rows))
            return 'Y'; // Won

        turn++;
    }

    return 'N'; // Lost
}
