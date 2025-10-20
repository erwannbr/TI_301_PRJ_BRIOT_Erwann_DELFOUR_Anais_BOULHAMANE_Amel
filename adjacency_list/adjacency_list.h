//
// Created by Boulhamane Amel on 20/10/2025.
//

#ifndef ADJACENCY_LIST_H
#define ADJACENCY_LIST_H
#include <stdlib.h>

typedef struct s_adjacency_list {
    t_std_list *list;
    int size;
}t_adjacency_list;

t_adjacency_list* empty_adjacency_list(int size) {
    t_adjacency_list *adjacency_list;
    adjacency_list.size = size;

    adjacency_list.list = create_std_list(size);
    if (adjacency_list.list == NULL) {
        return NULL;
    }
    for (int i = 0; i < size; i++) {}



}

#endif //ADJACENCY_LIST_H
