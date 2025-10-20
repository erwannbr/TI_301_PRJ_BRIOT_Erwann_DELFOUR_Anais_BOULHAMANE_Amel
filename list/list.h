//
// Created by anais on 20/10/2025.
//

#ifndef LIST_H
#define LIST_H

#include "../cell/cell.h"

typedef struct s_std_list{
    t_cell *head;
}t_std_list;

t_std_list createEmptyList(void);
void addCellToList(t_std_list *list, int arrival, float probability);
void DisplayList(t_std_list list);

#endif //LIST_H
