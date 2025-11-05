#ifndef LIST_H
#define LIST_H

#include "../cell/cell.h"

/**
 * @brief Standard linked list structure
 */
typedef struct s_std_list{
    t_cell *head;
}t_std_list;

/**
 * @brief Creates an empty list
 * @return Empty list with NULL head
 */
t_std_list createEmptyList(void);

/**
 * @brief Adds a cell at the beginning of the list
 * @param list Pointer to the list
 * @param arrival Destination vertex
 * @param probability Edge probability
 */
void addCellToList(t_std_list *list, int arrival, float probability);

/**
 * @brief Displays list contents
 * @param list List to display
 */
void DisplayList(t_std_list list);

#endif //LIST_H
