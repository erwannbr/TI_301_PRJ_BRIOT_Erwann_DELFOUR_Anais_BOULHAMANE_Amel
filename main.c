#include <stdio.h>
#include "adjacency_list/adjacency_list.h"



int main() {
    printf("Hello, World!\n");
    printf("it works");
    t_adjacency_list new_adj_list = readGraph("DATA/exemple1.txt");
    display_adjacency_list(new_adj_list);
    return 0;

}
