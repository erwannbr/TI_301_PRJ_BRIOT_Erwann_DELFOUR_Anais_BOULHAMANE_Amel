#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/utils.h"
#include "../adjacency_list/adjacency_list.h"

t_adjacency_list readGraph(const char *filename) {
    FILE *file = fopen(filename, "rt");
    if (!file) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    int nb_vertices, start, end;
    float proba;

    if (fscanf(file, "%d", &nb_vertices) != 1) {
        perror("Erreur de lecture du nombre de sommets");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    /* utilise la fonction demandée */
    t_adjacency_list *gp = empty_adjacency_list(nb_vertices);

    while (fscanf(file, "%d %d %f", &start, &end, &proba) == 3) {
        addCellToList(&gp->array[start - 1], end, proba);
    }

    fclose(file);

    /* on retourne par valeur; on libère juste le conteneur (pas le tableau) */
    t_adjacency_list g = *gp;  // copie superficielle : g.array reste valide
    free(gp);                  // OK : on garde g.array alloué
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
