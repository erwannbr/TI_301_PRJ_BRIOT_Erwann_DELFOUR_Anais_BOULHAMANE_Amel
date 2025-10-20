//
// Created by anais on 20/10/2025.
//
#include "../list.h"

t_std_list * CreateEmptyList () {
    t_std_list *list = (t_std_list*)malloc(sizeof(t_std_list));

    list->head = NULL;

    return list;
}

void DisplayList (t_std_list list) {
    t_cell* curr = list.head;
    printf("[head @]");

    while (curr->next != NULL) {
        printf ("(%d,%f) @->", curr->arrival,curr->probability);
        curr=curr->next;
    }
    
    printf ("(%d,%f)" , curr->arrival,curr->probability);

    return;
}

void AddCell(t_std_list *list, p_cell cell) {
    p_cell curr = list->head;

    if (curr == NULL){
        curr = curr->next;
    }

    curr->next = cell;
    cell->next = NULL;

    return;
}