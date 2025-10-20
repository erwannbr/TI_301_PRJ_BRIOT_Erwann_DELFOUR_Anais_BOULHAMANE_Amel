//
// Created by Boulhamane Amel on 20/10/2025.
//

t_adjacency_list* empty_adjacency_list(int size) {
    t_adjacency_list *p_adj_list = malloc(sizeof(t_adjacency_list));
    if (p_adj_list == NULL) {
        return NULL;
    }
    p_adj_list->size = size;
    p_adj_list->list = malloc(size*sizeof(t_std_list));
    if (p_adj_list->list == NULL) {
        return NULL;
    }
    for (int i = 0; i < size; i++) {
        p_adj_list->list[i].head = NULL;
    }
    return p_adj_list;
}

void display_adjacency_list(t_adjacency_list list) {
    for (int i = 0; i < list.size; i++) {
        printf("List for vertex %d : ", i + 1);
        DisplayList(list.array[i]);
        printf("\n");
    }
}
