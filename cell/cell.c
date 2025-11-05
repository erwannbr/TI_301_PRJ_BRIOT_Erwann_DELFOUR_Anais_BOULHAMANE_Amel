#include "cell.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Creates a new cell
 * @param arrival Arrival vertex
 * @param probability Edge probability
 * @return New cell or NULL on error
 */
t_cell *create_cell(int arrival, float probability) {
    t_cell *cell = (t_cell *)malloc(sizeof(t_cell));

    if (cell ==NULL) {
        return NULL;
    }

    cell->arrival = arrival;
    cell->probability = probability;
    
    return cell;
}


