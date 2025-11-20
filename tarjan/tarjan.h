//
// Created by anais on 03/11/2025.
//

#ifndef TARJAN_H
#define TARJAN_H

#include "../adjacency_list/adjacency_list.h"

typedef struct s_stack t_stack;


typedef struct s_tarjan_vertex {
    int id; //identifier: the vertex number in the graph (1, 2, 3, ...)
    int class_nb; //the number of the visit number (0 to ...)
    int link_nb; //smallest index reachable
    int in_stack; //test if its currently beeing processed (already in the stack so in the boucle)
} t_tarjan_vertex, *p_tarjan_vertex;

t_tarjan_vertex * CreateArr (int n);

typedef struct s_class {
    char name[10];
    int * vertices;
    int nb_vertices;
    int size;
}t_class, *p_class;

t_class * CreateClass (const char * name_of_class);
void AddVertexToClass (p_class c, int vertex_id);

typedef struct s_partition {
    t_class **classes;
    int nb_class;
    int size;
}t_partition, *p_partition;

t_partition *CreatePartition();
void AddClassToPartition (p_partition p, p_class c);

void Parcours (int ver, t_adjacency_list graph, p_tarjan_vertex Ver, t_stack *S, p_partition part, int *index);
p_partition tarjan (t_adjacency_list graph);

#endif //TARJAN_H
