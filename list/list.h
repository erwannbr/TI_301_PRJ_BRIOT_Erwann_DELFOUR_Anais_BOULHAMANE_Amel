//
// Created by anais on 20/10/2025.
//

#ifndef LIST_H
#define LIST_H
#include <stdio.h>
#include "cell/cell.h"
#include "utils/utils.h"

typedef struct s_std_list
{
    p_cell head;
} t_std_list;

t_std_list * CreateEmptyList ();
void DisplayList(t_std_list);
void AddCell(t_std_list*, p_cell);
#endif //LIST_H
