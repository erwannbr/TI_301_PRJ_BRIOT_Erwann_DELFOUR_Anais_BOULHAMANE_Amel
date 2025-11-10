#include "tarjan.h"
#include "../cell/cell.h"
#include "../utils/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

t_tarjan_vertex * CreateArr (int n) {
    t_tarjan_vertex * array = malloc (n * sizeof (t_tarjan_vertex));
    if (!array) {
        printf("Allocation failed\n");
        exit (EXIT_FAILURE);
    }
    for (int i=0; i<n; i++) {
        array[i].id = i+1;
        array[i].class_nb = -1;
        array[i].link_nb = -1;
        array[i].in_stack = 0;
    }
    return array;
}

p_class CreateClass (const char * name_of_class) {
    p_class c = malloc(sizeof(t_class));
    if (!c) {
        printf("Allocation of class failed\n");
        exit (EXIT_FAILURE);
    }
    strcpy(c->name, name_of_class);
    c->vertices = malloc (4 * sizeof (int));
    if (!c ->vertices) {
        free(c);
        printf("Allocation of vertices failed\n");
        exit(EXIT_FAILURE);
    }
    c->nb_vertices = 0;
    c->size = 4;
    return c;

}

void AddVertexToClass (p_class c, int vertex_id) {
    if (!c) return;
    if (c->nb_vertices >= c->size) {
        c->size *=2;
        int *temp = realloc(c->vertices, c->size * sizeof(int));
        if (!temp) {
            printf("Reallocation failed\n");
            exit(EXIT_FAILURE);
        }
        c->vertices = temp;
    }
    c->vertices[c->nb_vertices++] = vertex_id;
}

t_partition *CreatePartition() {
    t_partition *p = malloc(sizeof(t_partition));
    if (!p) {
        printf("Allocation of partition failed\n");
        exit (EXIT_FAILURE);
    }
    p->classes = malloc (4 * sizeof(t_class*));
    if (!p->classes) {
        free(p);
        printf("Allocation of classes array failed\n");
        exit(EXIT_FAILURE);
    }
    p->nb_class = 0;
    p->size = 4;
    return p;
}

void AddClassToPartition (p_partition p, p_class c) {
    if (!p || !c) return;
    if (p->nb_class >= p->size) {
        p->size *= 2;
        t_class **temp = realloc(p->classes, p->size * sizeof(t_class*));
        if (!temp) {
            printf("Reallocation failed\n");
            exit(EXIT_FAILURE);
        }
        p->classes = temp;
    }
    p->classes[p->nb_class++] = c;
}

void Parcours (int v, t_adjacency_list graph, p_tarjan_vertex Ver, t_stack *S, p_partition part, int *index) {
    if (!Ver || !S || !part || !index) return;
    Ver[v].class_nb = *index;
    Ver[v].link_nb = *index;
    (*index)++;
    push(S,v);
    Ver[v].in_stack=1;
    p_cell neigh = graph.array[v].head;

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
        snprintf(name, sizeof(name), "C%d", part->nb_class + 1);
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

p_partition tarjan (t_adjacency_list graph) {
    int n = graph.size;
    p_tarjan_vertex Ver = CreateArr(n);
    t_stack *S = CreateStack();
    if (!S) {
        free(Ver);
        printf("Stack creation failed\n");
        exit(EXIT_FAILURE);
    }
    p_partition part = CreatePartition();
    int index = 0;

    for (int i = 0; i < n; i++) {
        if (Ver[i].class_nb == -1) {
            Parcours(i, graph, Ver, S, part, &index);
        }
    }
    free(Ver);

    while (!isEmpty(S)) {
        pop(S);
    }
    free(S);
    return part;
}