#include <stdio.h>
#include <stdlib.h>
#include "adjacency_list/adjacency_list.h"
#include "hasse/hasse.h"
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
    printf("\n");

    printf("Part 2: step 2 validation :\n");

    int *vertex_to_class = create_array_vertex_to_class(
        example_valid_step3.size, partition);
    //initialize
    t_link_array class_links;
    links_init(&class_links);

    list_class_links(&example_valid_step3, vertex_to_class, &class_links);

    printf("Links between classes:\n");
    for (int i = 0; i < class_links.size; i++) {
        printf("  %s --> %s\n",
               partition->classes[class_links.links[i].start]->name,
               partition->classes[class_links.links[i].end]->name);
    }
    printf("\n");

    removeTransitiveLinks(&class_links);

    printf("Links after removing transitive ones:\n");
    for (int i = 0; i < class_links.size; i++) {
        printf("  %s --> %s\n",
               partition->classes[class_links.links[i].start]->name,
               partition->classes[class_links.links[i].end]->name);
    }
    printf("\n");

    //export
    printf("Hasse diagram (Mermaid format):\n");
    print_hasse_mermaid(partition, &class_links);
    printf("\n");


    return 0;
}

