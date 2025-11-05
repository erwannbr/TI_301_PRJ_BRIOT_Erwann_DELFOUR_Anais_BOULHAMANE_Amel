#ifndef CELL_H
#define CELL_H

/**
 * @brief Cell for graph adjacency list
 */
typedef struct s_cell {
    int arrival;
    float probability;
    struct s_cell *next;
}t_cell, *p_cell;

/**
 * @brief Creates a new cell
 * @param arrival Destination vertex
 * @param probability Edge probability
 * @return New cell or NULL on error
 */
t_cell *create_cell(int arrival, float probability);

#endif //CELL_H
