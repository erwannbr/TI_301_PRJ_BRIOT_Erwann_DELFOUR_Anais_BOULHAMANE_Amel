#include "tarjan.h"
#include "../cell/cell.h"
#include "../utils/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

t_tarjan_vertex * CreateArr (int n) {
    // allocate an array of n Tarjan vertices.
    // each element will store the state of one vertex during Tarjan's algorithm.
    t_tarjan_vertex * array = malloc (n * sizeof (t_tarjan_vertex));
    if (!array) {
        printf("Allocation failed\n");
        exit (EXIT_FAILURE);
    }
    // initialize each Tarjan vertex with default values
    for (int i=0; i<n; i++) {
        array[i].id = i+1; // identifier: vertex number in the graph (1..n)
        array[i].class_nb = -1; // Tarjan 'index' : -1 means "unvisited"
        array[i].link_nb = -1; // lowlink value; -1 means "not computed yet"
        array[i].in_stack = 0; // boolean: 0 = not in Tarjan's stack, 1 = in the stack
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
    if (Ver == NULL  || S == NULL || part == NULL || index == NULL) return;  // if any essential pointer is NULL, we cannot continue
    Ver[v].class_nb = *index;
    Ver[v].link_nb = *index; // lowlink initially equals the index
    (*index)++; // Increment global index for the next vertex
    push(S,v); // push the current vertex onto the Tarjan stack
    Ver[v].in_stack=1; // Mark v as being in the stack
    p_cell neigh = graph.array[v].head; // get the adjacency list (neighbors) of vertex v

    // explore all neighbors of v
    while (neigh != NULL) {
        int w = neigh->arrival -1;
        // Case 1: neighbor w has not been visited yet (class_nb == -1)
        if (Ver[w].class_nb == -1) {
            Parcours(w, graph, Ver, S, part, index);
            if (Ver[w].link_nb < Ver[v].link_nb)
                Ver[v].link_nb = Ver[w].link_nb;
        }
        // Case 2: neighbor w is already on the stack
        else if (Ver[w].in_stack) {
            if (Ver[w].class_nb < Ver[v].link_nb)
                Ver[v].link_nb = Ver[w].class_nb;
        }
        // Move to the next neighbor in the adjacency list
        neigh = neigh->next;
    }
    // If v is the root of a strongly connected component:
    // Its lowlink equals its DFS index (class_nb)
    if (Ver[v].link_nb == Ver[v].class_nb) {
        // Create a name for the new component, like "C1", "C2", ...
        char name[10];
        snprintf(name, sizeof(name), "C%d", part->nb_class + 1);
        // Create a name for the new component, like "C1", "C2", ...
        p_class cls = CreateClass(name);

        int w;
        // create a name for the new component, like "C1", "C2", ...
        do {
            w = pop(S); // Get top vertex from stack
            Ver[w].in_stack = 0; // Mark it as no longer in the stack
            AddVertexToClass(cls, Ver[w].id); // Add the vertex to the current class
        } while (w != v); // Stop when we have popped v

        AddClassToPartition(part, cls); // Add the completed class to the partition of the graph
    }
}

p_partition tarjan (t_adjacency_list graph) {
    int n = graph.size; // Number of vertices in the graph
    p_tarjan_vertex Ver = CreateArr(n); // Create and initialize the Tarjan vertex array
    t_stack *S = CreateStack(); // Create an empty stack used by Tarjan's algorithm
    if (!S) {
        free(Ver);
        printf("Stack creation failed\n");
        return NULL;
    }

    // Create an empty partition that will store all strongly connected components (classes)
    p_partition part = CreatePartition();
    // Global index used for Tarjan numbering (DFS index)
    int index = 0;

    for (int i = 0; i < n; i++) {
        if (Ver[i].class_nb == -1) {
            Parcours(i, graph, Ver, S, part, &index);
        }
    }
    // we no longer need the Tarjan vertex array: free it
    free(Ver);


    // ensure the stack is completely emptied, then free it
    while (!isEmpty(S)) {
        pop(S);
    }
    free(S);
    return part;
}