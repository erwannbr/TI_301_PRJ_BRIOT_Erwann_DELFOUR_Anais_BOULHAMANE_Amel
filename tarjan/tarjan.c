//
// Created by anais on 03/11/2025.
//
#include "tarjan.h"
#include "../cell/cell.h"
#include <stdio.h>
#include <stdlib.h>

t_tarjan_vertex * CreateArr (int n) {
    t_tarjan_vertex * array = malloc (n * sizeof (t_tarjan_vertex));
    if (!array) {
        printf("Allocation failed");
        return NULL;
    }
    for (int i=0; i<n; i++) {
        array[i].id = i+1;
        array[i].class_nb = -1;
        array[i].link_nb = -1;
        array[i].in_stack = 0;
    }
    return array;
}

t_class * CreateClass (char * name_of_class) {
    t_class * c = malloc(sizeof(t_class));
    if (!c) {
        printf("Allocation of class failed");
        return NULL;
    }
    c->name = name_of_class;
    c->vertices = malloc (4 * sizeof (int));
    c->nb_vertices = 0;
    c->size = 4;
    return c;

}

void AddVertexToClass (p_class c, int vertex_id) {
    if (c->nb_vertices >= c->size) {
        c->size *=2;
        c->vertices = realloc (c->vertices, c->size * sizeof(int));
    }
    c->vertices [c->nb_vertices++] = vertex_id;
    return;
}

t_partition *CreatePartition() {
    t_partition *p = malloc(sizeof(t_partition));
    if (!p) {
        printf("Allocation of partition failed");
        return NULL;
    }
    p->classes = malloc (4 * sizeof(t_class*));
    p->nb_class = 0;
    p->size = 4;
    return p;
}

void AddClassToPartition (p_partition p, p_class c) {
    if (p->nb_class >= p->size) {
        p->size *= 2;
        p->classes = realloc (p->classes, p->size * sizeof(t_class*));
    }
    p->classes [p->nb_class++] = c;
    return;
}

void Parcours (int v, t_adjacency_list graph, p_tarjan_vertex Ver, t_stack *S, p_partition part, int *index) {
    Ver[v].class_nb = *index;
    Ver[v].link_nb = *index;
    (*index)++;
    push(S,v);
    Ver[v].in_stack=1;
    t_cell *neigh = graph.array[v].head;

    while (neigh != NULL) {
        int w = neigh->arrival -1;
        if (Ver[w].class_nb == -1) {
            Parcours(w, graph, Ver, S, part, index);
            if (Ver[w].link_nb < Ver[v].link_nb)
                Ver[v].link_nb = Ver[w].link_nb;
        }
        else if (Ver[w].in_stack) {
            if (Ver[w].class_nb < Ver[v].link_nb)
                Ver[v].link_nb = Ver[w].class_nb;
        }
        neigh = neigh->next;
    }
    if (Ver[v].link_nb == Ver[v].class_nb) {
        char name[10];
        sprintf(name, "C%d", part->size + 1);
        p_class cls = CreateClass(name);

        int w;
        do {
            w = pop(S);
            Ver[w].in_stack = 0;
            AddVertexToClass(cls, Ver[w].id);
        } while (w != v);

        AddClassToPartition(part, cls);
    }
}

p_partition *Tarjan (t_adjacency_list graph) {
    int n = graph.size;
    p_tarjan_vertex Ver = CreateArr(n);
    t_stack *S = CreateStack();
    p_partition part = CreatePartition();
    int index = 0;

    for (int i = 0; i < n; i++) {
        if (Ver[i].class_nb == -1) {
            Parcours(i, graph, Ver, S, part, &index);
        }
    }
    free(Ver);
    free(S);
    return part;
}