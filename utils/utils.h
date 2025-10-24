#ifndef __UTILS_H__
#define __UTILS_H__
#include "../adjacency_list/adjacency_list.h"

t_adjacency_list readGraph(const char *filename);
void checkIfMarkov(t_adjacency_list list);
void exportToMermaid(t_adjacency_list graph, const char *filename);

#endif