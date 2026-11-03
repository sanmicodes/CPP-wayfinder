# C++ Pathfinding and Graph Optimisation Engine

# C++ Grid Pathfinding & Graph Optimisation Engine

A graph-based pathfinding engine implemented in C++, exploring shortest-path algorithms, minimum spanning trees, graph traversal and algorithmic performance.

The engine models a two-dimensional Cartesian grid as a weighted graph and compares two different approaches to navigating obstacles, that I have learnt about during my curricular studies in Decision mathematics: Dijkstra's Shortest Path Algorithm and Kruskal's Minimum Spanning Tree Algorithm.

## Features

- **Dijkstra's Algorithm:** Finds the minimum-cost route between two points using a priority queue.
- **Kruskal's Algorithm:** Constructs a Minimum Spanning Tree (MST) using a Disjoint Set Union (DSU) data structure, then retrieves a route through the tree.
- **Weighted Graph Representation:** Straight movements have a cost of 10, while diagonal movements have a cost of 14.
- **Obstacle Generation:** Randomly generates obstacles with configurable density.
- **Interactive Navigation:** Allows users to change start and goal positions, toggle obstacles and regenerate the grid.
- **Performance Analysis:** Measures execution time and algorithm-specific work units.
- **Diagonal Collision Prevention:** Prevents diagonal movement through blocked corners.

## Mathematical Foundations

The environment is represented as an undirected weighted graph

This allows the graph to be stored efficiently using flat vectors.

### 1. Dijkstra's Shortest Path Algorithm

Dijkstra's algorithm calculates the minimum-cost path from a starting vertex to a destination vertex.

The implementation uses a min-priority queue to process vertices in increasing order of their tentative distance.




This implementation terminates early once the destination vertex is settled.

### 2. Kruskal's Minimum Spanning Tree Algorithm

Kruskal's algorithm constructs a Minimum Spanning Tree by sorting edges according to their weights and repeatedly selecting edges that connect separate components.

A Disjoint Set Union (DSU) structure prevents cycles during construction.

The resulting MST contains exactly one path between any two connected vertices.

However, an important distinction is that **the path through an MST is not necessarily the shortest path between two vertices**.

This makes Kruskal useful as a comparison of alternative graph structures rather than a competing shortest-path algorithm.





## Performance Analysis

The engine records:

- Execution time in microseconds.
- Number of vertices settled by Dijkstra.
- Number of edges examined by Kruskal.
- Total route cost.

These measurements allow investigation into how different graph algorithms behave on the same environment.

**Note:** The work metrics measure different operations, so they should not be treated as directly equivalent. Execution-time measurements can also vary due to system load and random graph generation.

## Potential Future Development

- Implement A* search using admissible heuristics.
- Introduce configurable grid dimensions.
- Implement Prim's Minimum Spanning Tree algorithm.


## Project Objective

This project explores the relationship between graph theory, algorithm design and computational efficiency through a practical pathfinding engine I made in c++


