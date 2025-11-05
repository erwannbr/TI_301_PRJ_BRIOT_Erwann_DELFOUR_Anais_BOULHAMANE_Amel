#ifndef __UTILS_H__
#define __UTILS_H__
#include "../adjacency_list/adjacency_list.h"
#include "../tarjan/tarjan.h"

t_adjacency_list readGraph(const char *filename);
void checkIfMarkov(t_adjacency_list list);
void exportToMermaid(t_adjacency_list graph, const char *filename);

t_tarjan_vertex * GraphIntoTar (t_adjacency_list* graph);

typedef struct s_stack_node {
    int vertex_id;
    struct s_stack_node *next;
} t_stack_node;

typedef struct s_stack {
    t_stack_node *head;
}t_stack;

t_stack * CreateStack ();
void push (t_stack *s, int vertex_id);
int pop (t_stack *s);
int isEmpty (t_stack *s);

#endif