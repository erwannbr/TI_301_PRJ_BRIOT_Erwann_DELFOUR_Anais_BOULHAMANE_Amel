//
// Created by anais on 03/11/2025.
//

#ifndef TARJAN_H
#define TARJAN_H

#include "../utils/utils.h"
#include "../adjacency_list/adjacency_list.h"
#include "../cell/cell.h"

typedef struct s_tarjan_vertex {
    int id;
    int class_nb;
    int link_nb;
    int in_stack;
} t_tarjan_vertex, *p_tarjan_vertex;

t_tarjan_vertex * CreateArr (int n);

typedef struct s_class {
    char name[10];
    int * vertices;
    int nb_vertices;
    int size;
}t_class, *p_class;

t_class * CreateClass (char * name_of_class);
void AddVertexToClass (p_class c, int vertex_id);

typedef struct s_partition {
    t_class **classes;
    int nb_class;
    int size;
}t_partition, *p_partition;

t_partition *CreatePartition();
void AddClassToPartition (p_partition p, p_class c);

void Parcours (int ver, t_adjacency_list graph, p_tarjan_vertex Ver, t_stack *S, p_partition part, int *index);
p_partition *Tarjan (t_adjacency_list graph);

#endif //TARJAN_H
