#include "adjacency_list.h"
#include <stdio.h>
#include <stdlib.h>

t_adjacency_list* empty_adjacency_list(int size) {
    t_adjacency_list *p_adj_list = malloc(sizeof(t_adjacency_list));
    if (p_adj_list == NULL) {
        return NULL;
    }
    p_adj_list->size = size;
    p_adj_list->array = malloc(size*sizeof(t_std_list));
    if (p_adj_list->array == NULL) {
        return NULL;
    }
    for (int i = 0; i < size; i++) {
        p_adj_list->array[i].head = NULL;
    }
    return p_adj_list;
}

void display_adjacency_list(t_adjacency_list * p_adj_list) {
    if (p_adj_list == NULL) return;

    for (int i = 0; i < p_adj_list->size; i++) {
        printf("List for vortex %d : ", i + 1);
        DisplayList(p_adj_list->array[i]);
        printf("\n");
    }
}

