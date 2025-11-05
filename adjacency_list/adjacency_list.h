#ifndef ADJACENCY_LIST_H
#define ADJACENCY_LIST_H

#include "../list/list.h"


/**
 * @brief Graph represented as adjacency list
 */
typedef struct s_adjacency_list {
    t_std_list *array;
    int size;
}t_adjacency_list, *p_adjacency_list;

/**
 * @brief Creates an empty adjacency list
 * @param size Number of vertices (must be > 0)
 * @return Pointer to new list, NULL on error
 */
t_adjacency_list* empty_adjacency_list(int size);

/**
 * @brief Displays the adjacency list
 * @param p_adj_list Adjacency list to display
 */
void display_adjacency_list(p_adjacency_list p_adj_list);

#endif //ADJACENCY_LIST_H
