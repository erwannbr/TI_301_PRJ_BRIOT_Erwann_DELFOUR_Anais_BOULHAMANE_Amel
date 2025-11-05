#ifndef __HASSE_H__
#define __HASSE_H__
#include "../tarjan/tarjan.h"

typedef struct {
    int start;
    int end;
} t_link;

typedef struct {
    t_link *links;
    int size;
    int length;
} t_link_array;

typedef struct {
    int *neighbors;
    int neighbor_count;
} Vertex;

void links_init(t_link_array *a);
void links_add(t_link_array *a, int start, int end);

int *create_array_vertex_to_class(int vertex_count, const t_partition *P);

void list_class_links(int vertex_count, Vertex *graph, int *vertex_to_class, t_link_array *class_links);
void removeTransitiveLinks(t_link_array *p_link_array);

/**
 * @brief Creates a link array from the given partition and graph.
 *
 * @param part The partition of the graph.
 * @param graph The adjacency list representation of the graph.
 * @return The created link array.
 */

#endif