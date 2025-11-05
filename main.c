#include <stdio.h>
#include "adjacency_list/adjacency_list.h"
#include "utils/utils.h"
#include "tarjan/tarjan.h"


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

    printf("Part 2: step 1 validation :\n");
    p_partition partition = tarjan(example_valid_step3);

    for (int i = 0; i < partition->nb_class; i++) {
        p_class c = partition->classes[i];
        printf("Component %s: {", c->name);
        for (int j = 0; j < c->nb_vertices; j++) {
            printf("%d", c->vertices[j]);
            if (j < c->nb_vertices - 1) printf(",");
        }
        printf("}\n");
    }

    printf("Part 2: step 1 validation :\n");


    return 0;
}
