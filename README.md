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

The relaxation operation is:

\[
d(v)=\min(d(v),d(u)+w(u,v))
\]

A predecessor array is maintained to reconstruct the final route.

**Time complexity:**

\[
O((V+E)\log V)
\]

This implementation terminates early once the destination vertex is settled.

### 2. Kruskal's Minimum Spanning Tree Algorithm

Kruskal's algorithm constructs a Minimum Spanning Tree by sorting edges according to their weights and repeatedly selecting edges that connect separate components.

A Disjoint Set Union (DSU) structure prevents cycles during construction.

The resulting MST contains exactly one path between any two connected vertices.

However, an important distinction is that **the path through an MST is not necessarily the shortest path between two vertices**.

This makes Kruskal useful as a comparison of alternative graph structures rather than a competing shortest-path algorithm.

**Time complexity:**

\[
O(E\log E)
\]

This includes edge sorting, with near-constant amortised DSU operations.

## Implementation

| Component | Implementation |
|---|---|
| Language | C++17 |
| Graph representation | Flat vectors |
| Priority queue | `std::priority_queue` |
| Edge sorting | `std::stable_sort` |
| Connectivity | Disjoint Set Union |
| Graph traversal | Breadth-First Search |
| Timing | `std::chrono` |

## Installation & Execution

Clone the repository:

```bash
git clone YOUR_REPOSITORY_URL
cd YOUR_REPOSITORY_NAME
```

Compile:

```bash
g++ -std=c++17 -O2 wayfinder.cpp -o wayfinder
```

Run:

```bash
./wayfinder
```

## Controls

| Input | Function |
|---|---|
| 1 | Set goal |
| 2 | Generate random goal |
| 3 | Set start |
| 4 | Generate random obstacles |
| 5 | Toggle obstacle |
| 6 | Switch algorithm |
| 7 | Compare both algorithms |
| q | Quit |

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
- Compare adjacency lists against adjacency matrices.
- Investigate algorithmic scaling across different graph densities.
- Introduce automated benchmarking across multiple randomised environments.
- Explore graph optimisation and route-planning heuristics.

## Project Objective

This project explores the relationship between graph theory, algorithm design and computational efficiency through a practical pathfinding application.

It provides a foundation for further investigation into combinatorial optimisation, heuristic search and computational complexity.
