#ifndef __UTILS_H__
#define __UTILS_H__
#include "adjacency_list/adjacency_list.h"

t_adjacency_list readGraph(const char *filename) {
    FILE *file = fopen(filename, "rt"); // read-only, text
    int nbvert, start, end;
    float proba;
    t_adjacency_list *p_adj_list = empty_adjacency_list(nbvert);
    if (file == NULL)
    {
    perror("Could not open file for reading");
    exit(EXIT_FAILURE);
    }
    // first line contains number of vertices
    if (fscanf(file, "%d", &nbvert) != 1)
    {
    perror("Could not read number of vertices");
    exit(EXIT_FAILURE);
    }
    //Initialise an empty adjacency list using the number of vertices
    while (fscanf(file, "%d %d %f", &start, &end, &proba) == 3)
    {
    // we obtain, for each line of the file, the values
    // start, end and proba
    //Add the edge that runs from 'start' to ‘end’ with the probability 'proba' to the adjacency list
 } 

#endif