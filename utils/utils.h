#ifndef __UTILS_H__
#define __UTILS_H__

#include "../adjacency_list/adjacency_list.h"
#include "../tarjan/tarjan.h"

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

#endif
