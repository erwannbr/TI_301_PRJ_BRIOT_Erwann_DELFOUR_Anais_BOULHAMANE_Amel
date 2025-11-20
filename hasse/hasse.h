#ifndef __HASSE_H__
#define __HASSE_H__

#include "../tarjan/tarjan.h"

/**
 * @brief Represents a directed link between two classes.
 *
 * A link corresponds to a directed edge in the reduced Markov graph
 * or Hasse diagram, from class 'start' to class 'end'.
 */
typedef struct {
    int start;   /**< Index of source class */
    int end;     /**< Index of destination class */
} t_link;

/**
 * @brief Dynamic structure storing an array of directed links.
 *
 * The structure grows automatically when needed.
 * 'size' = number of used entries
 * 'length' = allocated capacity
 */
typedef struct {
    t_link *links; /**< Array of links */
    int size;      /**< Current number of links */
    int length;    /**< Allocated capacity */
} t_link_array, *p_link_array;

/**
 * @brief Creates an array mapping each vertex to the class it belongs to.
 *
 * @param vertex_count Total number of vertices in the graph
 * @param P Pointer to the partition (result of Tarjan or other SCC algorithm)
 * @return Pointer to an array of size vertex_count, or NULL on allocation error
 */
int *create_array_vertex_to_class(int vertex_count, const t_partition *P);

/**
 * @brief Computes the directed links between classes based on adjacency data.
 *
 * For every edge (u → v), this function adds a class‑level link
 * from class(u) to class(v) if they differ.
 *
 * @param G Pointer to adjacency list of the original graph
 * @param vertex_to_class Array that maps each vertex to its class index
 * @param class_links Output structure that accumulates class‑to‑class links
 */
void list_class_links(const t_adjacency_list *G, const int *vertex_to_class, t_link_array *class_links);

/**
 * @brief Exports a Hasse diagram (class graph) into a Mermaid diagram file.
 *
 * @param P Pointer to partition structure containing all classes
 * @param L Pointer to class link array
 * @param filename Path to the output file (.md, .mmd, ...)
 */
void print_hasse_mermaid(const t_partition *P, const t_link_array *L, const char *filename);

/**
 * @brief Initializes a t_link_array structure.
 * @param a Pointer to the link array to initialize
 */
void links_init(t_link_array *a);

/**
 * @brief Adds a link to the array, avoiding duplicates.
 *
 * Automatically grows the array when necessary.
 *
 * @param a Pointer to the dynamic link array
 * @param start Origin class index
 * @param end Destination class index
 */
void links_add(t_link_array *a, int start, int end);

/**
 * @brief Removes all links that are transitively implied by others.
 *
 * This performs a transitive reduction of the class graph.
 *
 * @param array Pointer to the link array
 */
void removeTransitiveLinks(t_link_array *array);

/**
 * @brief Determines whether a class is transient.
 *
 * A class is transient if it has at least one outgoing link
 * to a different class.
 *
 * @param class_id Index of the class
 * @param links Pointer to the class link array
 * @return 1 if the class is transient, 0 otherwise
 */
int is_class_transient(int class_id, t_link_array *links);

/**
 * @brief Determines whether a class is persistent.
 *
 * Persistent = not transient.
 *
 * @param class_id Index of the class
 * @param links Pointer to the class link array
 * @return 1 if persistent, 0 otherwise
 */
int is_class_persistent(int class_id, t_link_array *links);

/**
 * @brief Determines whether a given class corresponds to an absorbing state.
 *
 * A class is absorbing if:
 *   - it is persistent (no outgoing transitions to other classes)
 *   - and it contains exactly one vertex.
 *
 * @param class_id Index of the class
 * @param partition Pointer to the partition structure
 * @param links Pointer to class link array
 * @return 1 if absorbing, 0 otherwise
 */
int is_state_absorbing(int class_id, t_partition *partition, t_link_array *links);

/**
 * @brief Determines whether the global Markov graph is irreducible.
 *
 * Irreducible = only one class exists.
 *
 * @param partition Pointer to the partition structure
 * @return 1 if irreducible, 0 otherwise
 */
int is_graph_irreducible(t_partition *partition);

/**
 * @brief Displays information about each class and state:
 *        - transient/persistent
 *        - absorbing states
 *        - global reducibility
 *
 * @param partition Pointer to the partition structure
 * @param links Pointer to class links
 */
void display_graph_characteristics(t_partition *partition, t_link_array *links);

#endif
