#include <iostream> // Input/output stream functionality
#include <vector> // Dynamic array functionality using vectors
#include <string> // String manipulation functions
#include <limits> // Numeric limits and error handling
#include <sstream> // String stream functionality for parsing
#include "Labyrinth.h" // Include the declaration of the Labyrinth class

bool isCharAlreadyPresent(char** labyrinth, int row, int m, char target) {
    // Check if a character is already present in a given row
    for (int k = 0; k < m; k++) {
        // Check if the target character is present in the current row
        if (labyrinth[row][k] == target) {
            return true;
        }
    }
    // Return false if the target character is not found in the row
    return false;
}

int main() {
    std::vector<char> res;
    int instances;

    // Ask the user to enter the number of instances without allowing spaces
    while (true) {
        std::cout << "Enter the number of instances : ";
        std::string input;
        std::getline(std::cin, input);

        // Check for spaces and non-numeric characters in the input
        if (input.find_first_not_of("0123456789") == std::string::npos) {
            // Extract the number of instances
            std::istringstream iss(input);
            iss >> instances;

            // Validate user input for the number of instances
            if (iss.fail() || instances <= 0) {
                std::cin.clear();  // Clearing the error flag
                std::cerr << "\nInvalid input for the number of instances. Please enter a positive integer." << std::endl;
            } else {
                break;  // Exit the loop if the input is valid
            }
        } else {
            std::cerr << "\nError : Invalid characters or spaces in the input. Please enter a positive integer." << std::endl;
        }
    }

    char** labyrinth;
    int n = 0, m = 0;

    // Process labyrinth instances
    for (int i = 0; i < instances; i++) {
        // Ask the user to enter the labyrinth dimensions for the current instance
        while (true) {
            std::cout << "Enter the dimensions of the labyrinth for instance " << i + 1 << " (1-1000) (n m) : ";
            std::cin >> n >> m;

            if (std::cin.fail() || n < 1 || m < 1 || n > 1000 || m > 1000) {
                std::cin.clear();  // Clearing the error flag
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Discarding invalid input
                std::cerr << "\nInvalid input for labyrinth dimensions. Please try again." << std::endl;
            } else {
                break;  // Exit the loop if the input is valid
            }
        }

        labyrinth = new char*[n];
        bool hasD = false, hasF = false, hasS = false;

        // Allocate memory for the labyrinth based on user input for dimensions
        // Initialize variables to track occurrences of characters D, F, and S

        for (int j = 0; j < n; j++) {
            labyrinth[j] = new char[m];

            while (true) {
                // Ask the user to enter row data for the current instance
                std::string row;
                std::cout << "Enter row " << j + 1 << " of the labyrinth for instance " << i + 1 << " : ";
                std::cin >> row;

                // Validate user input for the labyrinth row
                if (row.length() != static_cast<size_t>(m) || std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cerr << "\nInvalid input for labyrinth row. Please try again." << std::endl;
                } else {
                    // Check if D, F, or S are already present in the row
                    if (isCharAlreadyPresent(labyrinth, j, m, 'D') && row.find('D') != std::string::npos) {
                        std::cerr << "\nError : Only one 'D' is allowed. Please try again." << std::endl;
                        continue;
                    }

                    if (isCharAlreadyPresent(labyrinth, j, m, 'F') && row.find('F') != std::string::npos) {
                        std::cerr << "\nError : Only one 'F' is allowed. Please try again." << std::endl;
                        continue;
                    }

                    if (isCharAlreadyPresent(labyrinth, j, m, 'S') && row.find('S') != std::string::npos) {
                        std::cerr << "\nError : Only one 'S' is allowed. Please try again." << std::endl;
                        continue;
                    }

                    // Create a new const char* and copy the data
                    const char* constRow = row.c_str();
                    for (int k = 0; k < m; k++) {
                        labyrinth[j][k] = constRow[k];

                        // Update the variables to track occurrences
                        if (labyrinth[j][k] == 'D') hasD = true;
                        if (labyrinth[j][k] == 'F') hasF = true;
                        if (labyrinth[j][k] == 'S') hasS = true;
                    }

                    break;  // Exit the loop if the input is valid
                }
            }
        }

        // Check that there is only one occurrence of D, F, and S
        if (!hasD || !hasF || !hasS) {
            std::cerr << "\nError : Each instance must have one 'D', one 'F', and one 'S'. Please try again." << std::endl;

            // Clean up memory
            for (int j = 0; j < n; j++) {
                delete[] labyrinth[j];
            }
            delete[] labyrinth;

            // Decrement the instance counter to re-enter the input for the same instance
            i--;
            continue;  // Skip the rest of the loop and start the next iteration
        }

        // Create the graph representing the labyrinth
        Labyrinth::Graph graph;
        int startV = 0, endV = 0;

        // Populate the graph with vertices and identify the start (D) and end (S) vertices
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < m; k++) {
                graph.addVertex(labyrinth[j][k], 1, j, k);
                if (labyrinth[j][k] == 'D')
                    startV = j * m + k;
                if (labyrinth[j][k] == 'S')
                    endV = j * m + k;
            }
        }

        // Add edges to the graph
        for (int row = 0; row < n; row++) {
            for (int col = 0; col < m; col++) {
                int startVertex = row * m + col;
                int destination;
                double weight;

                // Traverse direct and diagonal neighbors
                for (int y = -1; y <= 1; y++) {
                    for (int j = -1; j <= 1; j++) {
                        if ((y != 0) || (j != 0)) { // Exclude the vertex itself
                            // Check grid boundaries
                            if ((row + y) >= 0 && (row + y) < n && (col + j) >= 0 && (col + j) < m) {
                                destination = (row + y) * m + col + j;
                                weight = 1;

                                // Add the edge if it is horizontal or vertical
                                if (std::abs(startVertex - destination) == 1 || std::abs(startVertex - destination) == m)
                                    graph.addEdge(startVertex, destination, weight);
                            }
                        }
                    }
                }
            }
        }

        // Execute the instance and add the result to the list
        res.push_back(Labyrinth::runInstance(graph, startV, endV, n, m));

        // Memory cleanup
        for (int j = 0; j < n; j++) {
            delete[] labyrinth[j];
        }
        delete[] labyrinth;
    }

    // Display the result(s)
    std::cout << "Result(s) :" << std::endl;
    for (char c : res)
        std::cout << c << std::endl;

    return EXIT_SUCCESS;
}
