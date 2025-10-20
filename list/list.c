//
// Created by anais on 20/10/2025.
//
#include "list.h"

t_std_list createEmptyList(void) {
    t_std_list list;
    list.head = NULL;
    return list;
}

void DisplayList (t_std_list list) {
    t_cell* curr = list.head;
    printf("[head @] -> ");

    if (curr==NULL) {
        return;
    }
    while (curr->next != NULL) {
        printf ("(%d, %.2f) @-> ", curr->arrival,curr->probability);
        curr=curr->next;
    }
    
    printf ("(%d, %.2f)" , curr->arrival,curr->probability);

    return;
}

void addCellToList(t_std_list *list, int arrival, float probability) {
    t_cell *new_cell = create_cell(arrival, probability);
    new_cell->next = list->head;
    list->head = new_cell;
}