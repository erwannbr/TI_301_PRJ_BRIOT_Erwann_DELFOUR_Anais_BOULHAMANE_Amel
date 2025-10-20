//
// Created by Boulhamane Amel on 20/10/2025.
//

#ifndef CELL_H
#define CELL_H

typedef struct s_cell {
    int arrival;
    float probability;
    struct s_cell *next;
}t_cell;

t_cell *create_cell(int arrival, float probability);

#endif //CELL_H
