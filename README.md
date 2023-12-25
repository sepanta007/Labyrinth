# Labyrinth

* [Overview](#overview)
* [Features](#features)
* [Usage](#usage)
* [Licence](#licence)

## Overview

The Labyrinth project is a *C++* program designed to find the optimal path for a prisoner escaping from a labyrinth.
The labyrinth is represented as a weighted graph, where each vertex corresponds to a cell in the labyrinth, 
and edges represent possible movements between cells.

Note : For more details and additional information, please refer to the associated [PDF](https://github.com/sepanta007/Labyrinth/blob/master/labyrinth.pdf) document provided in French.

## Features

- **Graph Representation :** The labyrinth is modeled as a weighted graph, allowing for efficient pathfinding algorithms.

- **A\* Algorithm :** A* (A-star) algorithm is utilized for heuristic-based pathfinding, enhancing the efficiency of finding the optimal path.

- **Dijkstra's Algorithm :** The program employs Dijkstra's algorithm to find the shortest path from the prisoner's starting position to the exit.

- **Prisoner Movement :** The program simulates the movement of prisoners in the four cardinal directions at the same speed as fire.

- **Fire Propagation :** Additionally, the program simulates the spread of fire within the labyrinth, adding a dynamic element to the pathfinding process.

## Usage

**Compilation :**

```bash
   g++ -std=c++11 -Wall -Wextra -o labyrinth *.cpp -lm
```

The option `-lm` indicates to the compiler to link with the mathematical library (libm).

**Execution :**

```bash
   ./labyrinth
```

**Input :** The program will prompt you to enter the number of instances and the dimensions of the labyrinth for each instance. For each labyrinth instance, 
you'll be required to input a matrix representing the labyrinth with the following symbols:

- `D` : Represents the initial position of the prisoner.
- `F` : Represents the location of fire.
- `S` : Represents the exit or safe zone.
- `#` : Represents a wall or an obstacle.

The labyrinth matrix should be filled with dots (`.`) for open paths.

Note : 

- Ensure correct formatting and input validity.
- The labyrinth matrix should have a valid starting position (`D`), an exit (`S`), and a fire location (`F`).

**Output :** The program determines if the prisoner in each room has a chance of reaching the exit before succumbing to the fire.
The output character `Y` indicates a chance of pardon, while `N` signifies no chance.

## Licence

This project is licensed under the [MIT](https://github.com/sepanta007/Labyrinth/blob/master/LICENSE) License.
