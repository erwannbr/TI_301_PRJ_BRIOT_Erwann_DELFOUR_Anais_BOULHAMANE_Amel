#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/utils.h"
#include "../adjacency_list/adjacency_list.h"

t_adjacency_list readGraph(const char *filename) {
    FILE *file = fopen(filename, "rt");
    if (!file) {
        perror("Could not open file for reading");
        exit(EXIT_FAILURE);
    }

    int nb_vertices, start, end;
    float proba;

    if (fscanf(file, "%d", &nb_vertices) != 1) {
        perror("Could not read number of vertices");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    t_adjacency_list *gp = empty_adjacency_list(nb_vertices);

    while (fscanf(file, "%d %d %f", &start, &end, &proba) == 3) {
        addCellToList(&gp->array[start - 1], end, proba);
    }

    fclose(file);

    t_adjacency_list g = *gp;
    free(gp);
    return g;
}

static char *getID(int i)
{
    // translate from 1,2,3, .. ,500+ to A,B,C,..,Z,AA,AB,...
    static char buffer[10];
    char temp[10];
    int index = 0;

    i--; // Adjust to 0-based index
    while (i >= 0)
    {
        temp[index++] = 'A' + (i % 26);
        i = (i / 26) - 1;
    }

    // Reverse the string to get the correct order
    for (int j = 0; j < index; j++)
    {
        buffer[j] = temp[index - j - 1];
    }
    buffer[index] = '\0';

    return buffer;
}

void checkIfMarkov(t_adjacency_list list) {
    int is_markov = 1;

    for (int i = 0; i < list.size; i++) {
        float total_proba = 0;
        t_std_list current_list = list.array[i];
        t_cell * p_current_cell = current_list.head;

        while (p_current_cell != NULL) {
            total_proba += p_current_cell->probability;
            p_current_cell = p_current_cell->next;
        }

        // Check if this node's probabilities sum to 1
        if (total_proba < 0.99 || total_proba > 1.01) {
            is_markov = 0;
            break;
        }
    }

    if (is_markov) {
        printf("The graph is a Markov graph\n");
    } else {
        printf("The graph is not a Markov graph\n");
    }
}

void exportToMermaid(t_adjacency_list graph, const char *filename) {
    FILE *file = fopen(filename, "wt");
    if (file == NULL) {
        perror("Could not open file for writing");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "---\n");
    fprintf(file, "config:\n");
    fprintf(file, "   layout: elk\n");
    fprintf(file, "   theme: neo\n");
    fprintf(file, "   look: neo\n");
    fprintf(file, "---\n\n");

    fprintf(file, "flowchart LR\n");


    for (int i = 0; i < graph.size; i++) {
        fprintf(file, "%s((%d))\n", getID(i + 1), i + 1);
    }
    fprintf(file, "\n");


    for (int i = 0; i < graph.size; i++) {
        t_cell *current = graph.array[i].head;
        while (current != NULL) {
            fprintf(file, "%s -->|%.4f|%s\n", 
                   getID(i + 1), 
                   current->probability,  
                   getID(current->arrival));
            current = current->next;
        }
    }

    fclose(file);
    printf("Mermaid file '%s' generated successfully.\n", filename);
}
