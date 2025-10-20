//
// Created by anais on 20/10/2025.
//
#include "list.h"

t_std_list * CreateEmptyList () {
    t_std_list *list = (t_std_list*)malloc(sizeof(t_std_list));
    list.h = NULL;
    return list;
}

void Display

void AddCell(t_std_list * list, p_cell cell) {
    p_cell current = list->head;
    
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = cell;

    return; 
}