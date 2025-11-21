#ifndef __UTILS_H__
#define __UTILS_H__

#include "../adjacency_list/adjacency_list.h"
#include "../tarjan/tarjan.h"
#include "../hasse/hasse.h"
#include "../matrix/matrix.h"

/**
 * @brief Reads graph from file
 * @param filename Path to graph file
 * @return Adjacency list
 */
t_adjacency_list readGraph(const char *filename);

/**
 * @brief Checks if graph is Markov chain
 * @param list Adjacency list
 */
void checkIfMarkov(t_adjacency_list list);

/**
 * @brief Exports graph to Mermaid format
 * @param graph Graph to export
 * @param filename Output file
 */
void exportToMermaid(t_adjacency_list graph, const char *filename);

/**
 * @brief Converts graph to Tarjan array
 * @param graph Adjacency list pointer
 * @return Tarjan vertex array
 */
t_tarjan_vertex *GraphIntoTar(t_adjacency_list *graph);

/**
 * @brief Stack node
 */
typedef struct s_stack_node {
    int vertex_id;              /**< Vertex ID */
    struct s_stack_node *next;  /**< Next node */
} t_stack_node;

/**
 * @brief Stack structure
 */
typedef struct s_stack {
    t_stack_node *head;  /**< Top of stack */
} t_stack;

/**
 * @brief Creates empty stack
 */
t_stack *CreateStack();

/**
 * @brief Pushes vertex to stack
 */
void push(t_stack *s, int vertex_id);

/**
 * @brief Pops vertex from stack
 * @return Vertex ID or -1 if empty
 */
int pop(t_stack *s);

/**
 * @brief Checks if stack is empty
 * @return 1 if empty, 0 otherwise
 */
int isEmpty(t_stack *s);

/**
 * @brief Prints the connections between Strongly Connected Components (classes).
 * @param part The partition containing the classes.
 * @param links The array of links (edges) between these classes.
 */
void print_class(p_partition ,  t_link_array);

/**
 * @brief Prints the list of vertices contained in each class of the partition.
 * @param part The partition to display.
 */
void print_component(p_partition);

/**
 * @brief Raises a matrix to a specific power and prints the result.
 * @param M The matrix to multiply.
 * @param power The exponent to raise the matrix to.
 */
void PowerMatrix (p_matrix M, int power);

/**
 * @brief Computes the stationary matrix using iterative multiplication (M^k).
 *
 * Continues until the difference between iterations is less than epsilon.
 *
 * @param graph The graph to analyze.
 * @param epsilon The convergence threshold.
 * @param graph_name Name of the graph (for display purposes).
 */
void ComputeStationaryMatrix (t_adjacency_list graph, float epsilon, const char *graph_name);

/**
 * @brief Creates an identity matrix of size n.
 * @param n The dimension of the matrix.
 * @return Pointer to the identity matrix.
 */
p_matrix CreateIdentityMatrix(int);

/**
 * @brief Computes the linear combination of two matrices (alpha*A + (1-alpha)*B).
 *
 * Used for the Lazy Walk implementation (0.5*M + 0.5*I).
 *
 * @param A The first matrix.
 * @param B The second matrix.
 * @param alpha The mixing coefficient.
 * @return Pointer to the resulting matrix.
 */
p_matrix MixMatrices(p_matrix, p_matrix, float );

/**
 * @brief Solves for the stationary distribution vector.
 *
 * Handles periodic matrices by converting them to a Lazy Walk before solving.
 *
 * @param M The transition matrix.
 * @param period The period of the matrix (1 = aperiodic).
 */
void SolveStationaryDistribution(p_matrix M, int);

/**
 * @brief High-level analysis function: Finds SCCs, periods, and distributions.
 * @param graph The graph to analyze.
 */
void periodicity(t_adjacency_list);

/**
 * @brief Performs validation step 2: Classification of states.
 *
 * Identifies classes as Persistent or Transient and computes limits accordingly.
 *
 * @param graph The graph to validate.
 */
void step2_validation(t_adjacency_list );

#endif
