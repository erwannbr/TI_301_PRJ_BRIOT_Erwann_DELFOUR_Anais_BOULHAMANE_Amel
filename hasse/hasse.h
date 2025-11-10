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
} t_link_array, *p_link_array;

int *create_array_vertex_to_class(int vertex_count, const t_partition *P);
void list_class_links(const t_adjacency_list *G, const int *vertex_to_class, t_link_array *class_links);

void print_hasse_mermaid(const t_partition *, const t_link_array *, const char *);

void links_init(t_link_array *a);
void links_add(t_link_array *a, int start, int end);

void removeTransitiveLinks(t_link_array *);

int is_class_transient(int, t_link_array *);
int is_class_persistent(int, t_link_array *);
int is_state_absorbing(int , t_partition *, t_link_array *);
int is_graph_irreducible(t_partition *partition);
void display_graph_characteristics(t_partition *, t_link_array *);

#endif