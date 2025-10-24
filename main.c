#include <stdio.h>
#include "adjacency_list/adjacency_list.h"
#include "utils/utils.h"


int main() {
    t_adjacency_list exemple1 = readGraph("../DATA/exemple1.txt");
    t_adjacency_list example_valid_step3 = readGraph("../DATA/example_valid_step3.txt");

    printf("Step 1 validation :\n");
    display_adjacency_list(&exemple1);

    printf("Step 2 validation :\n");
    checkIfMarkov(exemple1);

    printf("Step 3 validation :\n");
    exportToMermaid(example_valid_step3, "../DATA/example_valid_step3.txt");
    
    return 0;
}
