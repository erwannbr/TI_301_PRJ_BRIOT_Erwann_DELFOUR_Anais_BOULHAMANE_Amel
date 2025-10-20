//
// Created by Boulhamane Amel on 20/10/2025.
//

// cell/cell.h
#ifndef CELL_H
#define CELL_H
#include <stdlib.h>
#include <stdio.h>

typedef struct s_cell {
    int arrival;
    float probability;
    struct s_cell *next;
}t_cell, *p_cell;

t_cell *create_cell(int arrival, float probability);

#endif //CELL_H
