#include "adjacency_list.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Creates an empty adjacency list
 * @param size Number of vertices in the graph (must be > 0)
 * @return Pointer to the created structure, NULL on error
 */
t_adjacency_list* empty_adjacency_list(int size) {
    // allocate memory for the adjacency list structure
    t_adjacency_list *p_adj_list = malloc(sizeof(t_adjacency_list));

    //check the creation
    if (p_adj_list == NULL) return NULL;

    p_adj_list->size = size; // store the number of vertices (size of the graph)
    p_adj_list->array = malloc(size*sizeof(t_std_list)); // allocate the array of lists, one list per vertex

    if (p_adj_list->array == NULL) {
        free(p_adj_list);
        return NULL;
    }
    // initialize each list in the array:
    // all head pointers are set to NULL (empty lists)
    for (int i = 0; i < size; i++) {
        p_adj_list->array[i].head = NULL;
    }
    return p_adj_list;
}

/**
 * @brief Displays the complete adjacency list
 * @param p_adj_list Pointer to the adjacency list
 */
void display_adjacency_list(p_adjacency_list p_adj_list) {
    //check if the p_adj_list is null or if the array is empty
    if (p_adj_list == NULL) {
        fprintf(stderr, "Error: NULL adjacency list\n");
        return;
    }

    if (p_adj_list->array == NULL) {
        fprintf(stderr, "Error: NULL adjacency list array\n");
        return;
    }

    //function to display the non-empty and existing p_adj_list
    for (int i = 0; i < p_adj_list->size; i++) {
        printf("List for vertex %d : ", i + 1);
        DisplayList(p_adj_list->array[i]);
        printf("\n");
    }
}

