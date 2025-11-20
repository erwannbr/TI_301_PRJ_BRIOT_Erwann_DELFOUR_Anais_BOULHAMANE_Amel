#include <stdio.h>
#include <stdlib.h>
#include "adjacency_list/adjacency_list.h"
#include "hasse/hasse.h"
#include "utils/utils.h"
#include "tarjan/tarjan.h"
#include "matrix/matrix.h"

int main() {
    float epsilon = 0.01;
    t_adjacency_list example1 = readGraph("DATA/exemple1.txt");
    t_adjacency_list example2 = readGraph("DATA/exemple2.txt");
    t_adjacency_list example3 = readGraph("DATA/exemple3.txt");
    t_adjacency_list example_valid_step3 = readGraph("DATA/exemple_valid_step3.txt");
    t_adjacency_list graph_meteo = readGraph("DATA/example_meteo.txt");


    printf("Part 1: step 1 validation :\n");
    display_adjacency_list(&example1);
    printf("\n");


    printf("Part 1: step 2 validation :\n");
    checkIfMarkov(example1);
    printf("\n");


    printf("Part 1: step 3 validation :\n");
    exportToMermaid(example_valid_step3, "DATA/mermaid-file.mmd");
    printf("\n");


    printf("Part 2: step 1 validation :\n");
    p_partition partition = tarjan(example_valid_step3);
    print_component(partition);


    printf("Part 2: step 2 validation :\n");
    int *vertex_to_class = create_array_vertex_to_class(example_valid_step3.size, partition);
    t_link_array class_links;
    links_init(&class_links);
    list_class_links(&example_valid_step3, vertex_to_class, &class_links);
    //possibility to print the classes
    //print_class(partition, class_links);

    printf("Hasse diagram (Mermaid format):\n");
    print_hasse_mermaid(partition, &class_links,"DATA/mermaid-file-part2.mmd");
    printf("\n");


    printf("Part 2: step 3 validation :\n");
    display_graph_characteristics(partition, &class_links);


    printf("Part 3: step 1 validation:\n");
    p_matrix  Meteo = CreateMatFromAdjList(graph_meteo);
    printf ("Meteo Matrix");
    printMatrix(Meteo);
    PowerMatrix (Meteo, 3);
    PowerMatrix (Meteo, 7);

    printf("Part 3: step 2 validation:\n");
    ComputeStationaryMatrix (example1, epsilon, "Example 1");
    ComputeStationaryMatrix (example2, epsilon, "Example 2");
    ComputeStationaryMatrix (example3, epsilon, "Example 3");
    for (int i = 0; i<2 ; i++) {
        Meteo = MultiplyMatrices(Meteo, Meteo);
    }
    printf("Meteo Matrix Power 3\n");
    printMatrix(Meteo);
    for (int j=0; j<4; j++) {
        Meteo = MultiplyMatrices(Meteo, Meteo);
    }
    printf("Meteo Matrix Power 7\n");
    printMatrix(Meteo);

    p_matrix M = CreateMatFromAdjList(graph_meteo);
    p_partition P = tarjan(graph_meteo);
    int *map = create_array_vertex_to_class(graph_meteo.size, P);
    t_link_array L;
    links_init(&L);
    list_class_links(&graph_meteo, map, &L);

    const int MAX_IT = 1000;
    const float EPS = 1e-6f;

    for (int c = 0; c < P->nb_class; ++c) {
        p_class cls = P->classes[c];
        int persistent = is_class_persistent(&L, c);

    printf("Part 3: step 2 validation:");
    periodicity(graph_meteo);
    return 0;
}
        printf("Classe C%d (", c+1);
        for (int k = 0; k < cls->nb_vertices; ++k) {
            if (k) printf(",");
            printf("%d", cls->vertices[k]);
        }
        printf(") : %s\n", persistent ? "persistante" : "transitoire");

        p_matrix S = SubMatrixByComponent(M, *P, c);
        if (!S) { printf("  (Sous-matrice introuvable)\n"); continue; }

        if (!persistent) {
            printf("  Distribution limite: [");
            for (int j = 0; j < S->size; ++j) printf("%s0", j ? " " : "");
            printf("]\n");
        } else {
            float *pi = StationaryVectorFromSubmatrix(S, MAX_IT, EPS);
            if (!pi) {
                printf("  Erreur calcul stationnaire\n");
            } else {
                printf("  Distribution stationnaire ~ [");
                for (int j = 0; j < S->size; ++j) printf("%s%.6f", j ? " " : "", pi[j]);
                printf("]\n");
                free(pi);
            }
        }
}