//
// Created by Boulhamane Amel on 20/10/2025.
//
#ifndef ADJACENCY_LIST_H
#define ADJACENCY_LIST_H
#include "../list/list.h"
#include <stdbool.h>
#include <stdlib.h>


typedef struct s_adjacency_list {
    t_std_list *array;
    int size;
}t_adjacency_list;

t_adjacency_list* empty_adjacency_list(int size);
void display_adjacency_list(t_std_list list);

#endif //ADJACENCY_LIST_H
