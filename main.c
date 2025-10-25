#include <stdio.h>
#include "adjacency_list/adjacency_list.h"
#include "utils/utils.h"


int main() {
    t_adjacency_list exemple1 = readGraph("DATA/exemple1.txt");
    t_adjacency_list example_valid_step3 = readGraph("DATA/exemple_valid_step3.txt");

    printf("Part 1: step 1 validation :\n");
    display_adjacency_list(&exemple1);
    printf("\n");

    printf("Part 1: step 2 validation :\n");
    checkIfMarkov(exemple1);
    printf("\n");

    printf("Part 1: step 3 validation :\n");
    exportToMermaid(example_valid_step3, "DATA/mermaidfile.mmd");
    printf("\n");
    
    return 0;
}
