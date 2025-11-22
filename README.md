# TI_301_PRJ_BRIOT_Erwann_DELFOUR_Anais_BOULHAMANE_Amel

# 📋 Project Overview
This project implements a comprehensive study of Markov Graphs (probability graphs) in C, covering graph representation, analysis, and probabilistic calculations.

# 🎯 Project Structure
The project is divided into 3 main parts:

 - Part 1: Graph Representation & Visualization

Load Markov graphs from text files
Store graphs using adjacency lists
Validate probability constraints (sum = 1)
Generate Mermaid diagrams for visualization

 - Part 2: Graph Analysis (Tarjan's Algorithm)

Partition graphs into strongly connected components (classes)
Identify transient and persistent classes
Detect absorbing states
Generate Hasse diagrams
Determine if graph is irreducible

 - Part 3: Probability Distributions

Calculate distribution evolution over discrete time
Matrix operations (multiplication, powers)
Find stationary distributions
BONUS: Calculate periodicity of classes


# 📁 File Structure
project/  
│  
├── adjacency_list/
│   ├── adjacency_list.c          # Adjacency list implementation  
│   └── adjacency_list.h          # Adjacency list structures & prototypes  
│  
├── cell/  
│   ├── cell.c                    # Cell (edge) operations  
│   └── cell.h                    # Cell structure definition   
│  
├── list/  
│   ├── list.c                    # Linked list implementation  
│   └── list.h                    # List structures & prototypes  
│  
├── matrix/  
│   ├── matrix.c                  # Matrix operations (multiply, power, etc.)  
│   └── matrix.h                  # Matrix structure & prototypes  
│   
├── tarjan/   
│   ├── tarjan.c                  # Tarjan's algorithm implementation  
│   └── tarjan.h                  # Tarjan structures & prototypes  
│  
├── hasse/  
│   ├── hasse.c                   # Hasse diagram generation  
│   └── hasse.h                   # Hasse diagram prototypes  
│  
├── utils/  
│   ├── utils.c                   # Helper/utility functions  
│   └── utils.h                   # Utility function prototypes  
│  
├── DATA/  
│   ├── example_meteo.txt         # Weather example  
│   ├── example_hasse1.txt        # Hasse diagram test  
│   ├── example_scc1.txt          # SCC test  
│   ├── example_valid_step3.txt   # 10-vertex validation  
│   ├── example1_chatGPT_fix...   # ChatGPT generated test  
│   ├── example1_from_chatg...    # Additional test case  
│   ├── example1.txt              # Basic test  
│   ├── example2.txt              # Test case 2  
│   ├── example3.txt              # Test case 3  
│   └── example4_2check.txt       # Verification test  
│  
├── CMakeLists.txt                # CMake configuration  
├── main.c                        # Program entry point  
└── README.md                     # This file  
  

# 🛠️ Compilation & Usage
Requires Cmake or gcc

# 📊 Input File Format
<number_of_vertices>  
<start> <end> <probability>  
<start> <end> <probability>  
...  
Example (4 vertices):  
4  
1 1 0.95  
1 2 0.04  
1 3 0.01  
2 2 0.90  
2 3 0.05  
2 4 0.05  
3 3 0.80  
3 4 0.20  
4 1 1.00  

# 🔍 Features by Part

 - Part 1 Features
✅ Data Structures

t_cell: Edge with destination vertex & probability
t_list: Linked list of edges
t_adjacency_list: Array of lists (one per vertex)

✅ Functions Implemented

createCell() - Create an edge cell
createEmptyList() - Initialize empty list
addCell() - Add edge to list
displayList() - Display adjacency list
readGraph() - Load graph from file
isMarkovGraph() - Validate probability sums
generateMermaidFile() - Export to Mermaid format

📤 Output

Console display of adjacency list
Validation messages
.mmd file for Mermaid visualization


 - Part 2 Features
✅ Tarjan's Algorithm Implementation

t_tarjan_vertex: Vertex metadata (id, number, accessible, in_stack)
t_partition: Set of strongly connected components
tarjan() - Main algorithm function
parcours() - Recursive traversal function

✅ Graph Classification

getClassType() - Identify transient/persistent classes
isAbsorbing() - Detect absorbing states
isIrreducible() - Check for single component

✅ Hasse Diagram

findClassLinks() - Determine inter-class connections
removeTransitiveLinks() - Remove redundant edges
generateHasseDiagram() - Create Hasse diagram in Mermaid

📤 Output
Component C1: {1,7,5}
Component C2: {2}
Component C3: {3,8,6}
...
Class {1,7,5} is TRANSIENT
Class {2} is PERSISTENT
State 2 is ABSORBING
Graph is NOT IRREDUCIBLE

 - Part 3 Features
✅ Matrix Operations

createMatrixFromGraph() - Convert adjacency list to matrix
createEmptyMatrix() - Initialize n×n zero matrix
copyMatrix() - Copy matrix values
multiplyMatrices() - Matrix multiplication
matrixDifference() - Calculate Σ|mᵢⱼ - nᵢⱼ|
matrixPower() - Calculate M^n

✅ Distribution Calculations

createDistribution() - Initialize probability vector
evolveDistribution() - Calculate Π₀ × M^n
findStationaryDistribution() - Find limit distribution
subMatrix() - Extract class submatrix

✅ Bonus: Periodicity

gcd() - Greatest common divisor
getPeriod() - Calculate class period
findPeriodicDistributions() - Multiple stationary states

# 📤 Output
Initial distribution: Π₀ = (1 0 0 0 0)  
After 3 days: Π₃ = (0.17 0.37 0.13 0.05 0.27)
After 7 days: Π₇ = (0.16 0.36 0.13 0.05 0.29)

Stationary distribution: Π* = (0.16 0.36 0.13 0.05 0.29)
Convergence reached after 7 iterations

Class {1,2,3} has period: 3

📈 Example: Weather Graph
Graph Structure (5 states)

Sunny
Cloudy
Rain
Storm
Sunny spells

Transition Matrix M  
      1     2     3     4     5  
1  [0.30  0.40  0.20  0.05  0.05]   
2  [0.20  0.30  0.30  0.10  0.10]  
3  [0.10  0.20  0.30  0.20  0.20]  
4  [0.10  0.15  0.25  0.30  0.20]  
5  [0.25  0.25  0.10  0.05  0.35]  
Question: "If sunny today, what's probability of cloudy in 3 days?"
Calculation:

Initial: Π₀ = (1, 0, 0, 0, 0)
Result: Π₃ = Π₀ × M³ = (0.17, 0.37, 0.13, 0.05, 0.27)

Answer: 37% chance of cloudy weather

# 🧪 Testing & Validation
Part 1 Validation

✅ Load exemple_meteo.txt
✅ Verify probability sums = 1.0 (±0.01)
✅ Generate valid Mermaid diagram

Part 2 Validation

✅ Correct partition: {1,5,7}, {2}, {3,6,8}, {4}, {9}, {10}
✅ Identify all class types correctly
✅ Generate Hasse diagram with proper hierarchy

Part 3 Validation

✅ M³ matches expected values
✅ M⁷ shows convergence to stationary distribution
✅ Difference |M^n - M^(n-1)| < ε


🎨 Visualization with Mermaid
Online Tool
https://www.mermaidchart.com/
How to Use

Run program to generate .mmd file
Copy file contents
Paste into Mermaid editor
View interactive graph

Example Output
flowchart LR
A((1)) -->|0.95|A
A -->|0.04|B
A -->|0.01|C
B((2)) -->|0.90|B
B -->|0.05|C
B -->|0.05|D
C((3)) -->|0.80|C
C -->|0.20|D
D((4)) -->|1.00|A

# 🤖 AI Tools Used
Tools

ChatGPT / Claude for algorithm explanations and claud eonly for code (best model outthere)
GitHub Copilot for code suggestions

Code Integration

AI-generated code was reviewed and adapted
All functions tested with provided examples
Comments added for clarity


# ⚠️ Known Issues & Limitations
Convergence Detection

Some graphs don't converge (periodic classes)
Epsilon threshold (0.01) may not work for all cases

Memory Management

Large matrices (>1000 vertices) may cause performance issues
Dynamic allocation for all structures

Floating Point Precision

Probability sums checked with tolerance ±0.01
May accumulate errors in long calculations


# 📚 Mathematical Concepts
Key Equations
Distribution Evolution:
Πₙ = Π₀ × Mⁿ
Stationary Distribution:
Π* × M = Π*
Matrix Difference:
diff(M, N) = Σᵢ Σⱼ |mᵢⱼ - nᵢⱼ|

 # 👥 Authors

Student Name: Erwann Briot, Anais Delfour, Amel Boulhamane
Course: TI301 - Algorithms and Data Structures 2
Institution: Efrei
Academic Year: 2024-2025


# 📝 License
This project is part of academic coursework. All rights reserved.


