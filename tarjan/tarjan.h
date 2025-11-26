//
// Created by anais on 03/11/2025.
//

#ifndef TARJAN_H
#define TARJAN_H

#include "../adjacency_list/adjacency_list.h"

typedef struct s_stack t_stack;


/**
 * @brief Structure tracking the state of a specific vertex during the algorithm.
 *
 * Stores the discovery index, the low-link value, and whether the vertex
 * is currently in the recursion stack.
 */
typedef struct s_tarjan_vertex {
    int id; //identifier: the vertex number in the graph (1, 2, 3, ...)
    int class_nb; //the number of the visit number (0 to ...)
    int link_nb; //smallest index reachable
    int in_stack; //test if its currently beeing processed (already in the stack so in the boucle)
} t_tarjan_vertex, *p_tarjan_vertex;

/**
 * @brief Allocates an array of Tarjan vertex states.
 * @param n The number of vertices to allocate.
 * @return Pointer to the allocated array.
 */
t_tarjan_vertex * CreateArr (int n);

/**
 * @brief Represents a Strongly Connected Component (Class).
 *
 * Holds a dynamic list of vertex IDs that belong to this specific component.
 */

typedef struct s_class {
    char name[10];
    int * vertices;
    int nb_vertices;
    int size;
}t_class, *p_class;

/**
 * @brief Creates a new class instance.
 * @param name_of_class The label for the class.
 * @return Pointer to the new class.
 */
t_class * CreateClass (const char * name_of_class);

/**
 * @brief Adds a vertex to a specific class.
 * @param c The class to modify.
 * @param vertex_id The ID of the vertex to add.
 */
void AddVertexToClass (p_class c, int vertex_id);

/**
 * @brief Represents the result of the algorithm: a partition of the graph.
 *
 * Contains a collection of classes (SCCs).
 */
typedef struct s_partition {
    t_class **classes;
    int nb_class;
    int size;
}t_partition, *p_partition;

/**
 * @brief Creates a new empty partition.
 * @return Pointer to the new partition.
 */
t_partition *CreatePartition();

/**
 * @brief Adds a class to the partition.
 * @param p The partition to modify.
 * @param c The class to add.
 */
void AddClassToPartition (p_partition p, p_class c);

/**
 * @brief Recursive DFS step (StrongConnect) for Tarjan's algorithm.
 * @param ver The current vertex being visited.
 * @param graph The graph structure.
 * @param Ver The array of vertex states.
 * @param S The recursion stack.
 * @param part The partition to store results.
 * @param index Pointer to the global discovery index.
 */
void Parcours (int ver, t_adjacency_list graph, p_tarjan_vertex Ver, t_stack *S, p_partition part, int *index);

/**
 * @brief Main entry point for computing Strongly Connected Components.
 * @param graph The graph to analyze.
 * @return A partition containing all found SCCs.
 */
p_partition tarjan (t_adjacency_list graph);

#endif //TARJAN_H
