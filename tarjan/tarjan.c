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


//most important function of tarjan (it will allow to go from the matrix with some adjancencies to some classes (which tarjan will use to return a partition(so a group of classes)
//so parcours is the function that will test all the possible path to see if they are forming classes (are doing a cicle (1->3->5->1))
void Parcours (int currvertex, t_adjacency_list graph, p_tarjan_vertex Ver, t_stack *stack, p_partition partition, int *index) {

    // if no tarjan_vertex OR no stack OR no partition OR no index --> return
    if (Ver == NULL  || stack == NULL || partition == NULL || index == NULL) return;

    //we initialize the current class number (so the order of visit at 1)
    Ver[currvertex].class_nb = *index;
    //we intialize the smallest index reachable to imself (since there is no one else)
    Ver[currvertex].link_nb = *index;
    //we increment the index to go to the next vertex
    (*index)++;
    //we push the current element in the stack since its currently processing (in the stack)
    push(stack,currvertex);
    //put the current vertex in stack so we put the in_stack to 1.
    Ver[currvertex].in_stack=1; // Mark v as being in the stack
    //we get from the adjacency list all the neighbors of the current vertex
    p_cell neigh = graph.array[currvertex].head;

    //we explore all the neighborgs of the current vortex (ALL)
    while (neigh != NULL) {
        int currentneigh = neigh->arrival -1;
        //case 1: the neighborgs is new so we recurcivly call parcours to cherche the neighborgs of the neighborg of vertex
        if (Ver[currentneigh].class_nb == -1) {
            Parcours(currentneigh, graph, Ver, stack, partition, index);
            //we check if the recursion manage to find a path to an ancestor if so than the current vertex have a path to an ancestor
            if (Ver[currentneigh].link_nb < Ver[currvertex].link_nb)
                Ver[currvertex].link_nb = Ver[currentneigh].link_nb;
        }
        //case 2: the neighborg is in the stack (means that currentneigh is an ancestor of current vertex
        else if (Ver[currentneigh].in_stack) {
            //if the class number (so the number of the visit is smaller than the link number of the
            if (Ver[currentneigh].class_nb < Ver[currvertex].link_nb)
                Ver[currvertex].link_nb = Ver[currentneigh].class_nb;
        }
        //move to the next neigh
        neigh = neigh->next;
    }

    //if current vertex is the root of a strongly connected component
    //in otehr word we cant reach a higher or lower than itself in the tree OR that the current vertex is entry point of a class
    //Create the class and put it in the partition.
    if (Ver[currvertex].link_nb == Ver[currvertex].class_nb) {
        //create a name for the component
        char name[10];
        snprintf(name, sizeof(name), "C%d", partition->nb_class + 1);
        //create a name for the newclass
        p_class newclass = CreateClass(name);

        int w;
        //create name and building the class by popping the stack
        do {
            w = pop(stack); // Get top vertex from stack
            Ver[w].in_stack = 0; // Mark it as no longer in the stack
            AddVertexToClass(newclass, Ver[w].id); // Add the vertex to the current class
        } while (w != currvertex); // Stop when we have popped v

        //we add the newclass to the top
        AddClassToPartition(partition, newclass); // Add the completed class to the partition of the graph
    }
}


//tarjan is the main function. it will use parcours to setup a partition (group of classes) and will do the allocation of memory etc.
//its like a support for his sub-function parcours. but parcours is more important.
p_partition tarjan (t_adjacency_list graph) {
    int n = graph.size; //get the size of the graph (of size n)
    p_tarjan_vertex Ver = CreateArr(n); //create the tarjan vertex array
    t_stack *Stack = CreateStack(); //create the stack for the parcours function

    //check if the rceation worked
    if (!Stack) {
        free(Ver);
        printf("Stack creation failed\n");
        return NULL;
    }

    //create the empty partition that will store the classes found in the parcours function
    p_partition part = CreatePartition();
    //the global index
    int index = 0;

    //the main algo, if the tarjan function (function that create the partition)
    for (int i = 0; i < n; i++) {
        if (Ver[i].class_nb == -1) {
            Parcours(i, graph, Ver, Stack, part, &index);
        }
    }

    //free to be sure that we got no leak of memory + free the stack to restart a new one
    free(Ver);
    while (!isEmpty(Stack)) {
        pop(Stack);
    }
    free(Stack);

    return part;
}