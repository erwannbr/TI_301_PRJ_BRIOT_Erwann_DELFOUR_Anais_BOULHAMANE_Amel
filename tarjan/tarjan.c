//
// Created by anais on 03/11/2025.
//
#include "tarjan.h"
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

void AddVertexToClass (t_class *c, int vertex_id) {
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

void AddClassToPartition (t_partition *p, t_class *c) {
    if (p->nb_class >= p->size) {
        p->size *= 2;
        p->classes = realloc (p->classes, p->size * sizeof(t_class*));
    }
    p->classes [p->nb_class++] = c;
    return;
}