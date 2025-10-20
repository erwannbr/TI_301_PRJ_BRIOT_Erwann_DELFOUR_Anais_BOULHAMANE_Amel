//
// Created by Boulhamane Amel on 20/10/2025.
//

#include "cell.h"

#include <stdlib.h>

t_cell *create_cell(int arrival, float probability) {
    t_cell *cell = (t_cell *)malloc(sizeof(t_cell));
    if (cell ==NULL) {
        return NULL;
    }
    cell->arrival = arrival;
    cell->probability = probability;
    return cell;
}
