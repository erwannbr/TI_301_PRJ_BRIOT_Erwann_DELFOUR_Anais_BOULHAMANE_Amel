#include "list.h"
#include <stdio.h>

/**
 * @brief Creates an empty list
 * @return Empty list with NULL head
 */
t_std_list createEmptyList(void) {
    t_std_list list;
    list.head = NULL; // initialize the head pointer to NULL, this means the list contains no edges yet
    return list;
}

/**
 * @brief Displays the list contents
 * @param list List to display
 * @note Format: [head @] -> (arrival, probability) @-> ... (arrival, probability)
 */
void DisplayList (t_std_list list) {
    t_cell* curr = list.head;
    printf("[head @] -> ");

    if (curr==NULL) {
        return;
    }
    // traverse the list until the second-to-last element
    // for each cell, print its arrival vertex and probability
    while (curr->next != NULL) {
        printf ("(%d, %.2f) @-> ", curr->arrival,curr->probability);
        curr=curr->next;
    }
    // print the last cell (without the arrow)
    printf ("(%d, %.2f)" , curr->arrival,curr->probability);

    return;
}

/**
 * @brief Adds a cell at the beginning of the list
 * @param list Pointer to the list
 * @param arrival Destination vertex
 * @param probability Edge probability
 * @note Inserts at head (O(1) operation)
 */
void addCellToList(t_std_list *list, int arrival, float probability) {
    t_cell *new_cell = create_cell(arrival, probability);
    new_cell->next = list->head;
    list->head = new_cell;
}