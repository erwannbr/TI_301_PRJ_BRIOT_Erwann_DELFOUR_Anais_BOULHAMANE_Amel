#include "hasse.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void links_init(t_link_array *a) {
    a->links = NULL;
    a->size = 0;
    a->length = 0;
}

void links_add(t_link_array *a, int start, int end) {
    for (int i = 0; i < a->size; ++i) {
        if (a->links[i].start == start && a->links[i].end == end) return;
    }
    if (a->size == a->length) {
        a->length = (a->length == 0) ? 8 : a->length * 2;
        a->links = (t_link*)realloc(a->links, a->length * sizeof *a->links);
    }
    a->links[a->size++] = (t_link){ start, end };
}

int *create_array_vertex_to_class(int vertex_count, const t_partition *partition) {
    int *vertex_to_class = (int*)malloc(vertex_count * sizeof *vertex_to_class);
    if (!vertex_to_class) return NULL;

    for (int vertex = 0; vertex < vertex_count; ++vertex)
        vertex_to_class[vertex] = -1;

    for (int class_index = 0; class_index < partition->nb_class; ++class_index) {
        const t_class *current_class = partition->classes[class_index];

        for (int i = 0; i < current_class->nb_vertices; ++i) {
            int vertex_id_1based = current_class->vertices[i];
            int vertex_index = vertex_id_1based - 1;

            if (vertex_index >= 0 && vertex_index < vertex_count)
                vertex_to_class[vertex_index] = class_index;
        }
    }

    return vertex_to_class;
}

void list_class_links(const t_adjacency_list *graph, const int *vertex_to_class, t_link_array *class_links)
{
    for (int source_vertex = 0; source_vertex < graph->size; ++source_vertex) {
        int source_class = vertex_to_class[source_vertex];

        p_cell neighbor_cell = graph->array[source_vertex].head;
        while (neighbor_cell != NULL) {
            int neighbor_id_1based = neighbor_cell->arrival;
            int neighbor_index = neighbor_id_1based - 1;

            if (neighbor_index >= 0 && neighbor_index < graph->size) {
                int destination_class = vertex_to_class[neighbor_index];

                if (source_class != destination_class) {
                    links_add(class_links, source_class, destination_class);
                }
            }

            neighbor_cell = neighbor_cell->next;
        }
    }
}

void print_hasse_mermaid(const t_partition *P, const t_link_array *L, const char *filename)
{
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

    fprintf(file,"flowchart TD\n");
    for (int c = 0; c < P->nb_class; ++c) {
        const t_class *cls = P->classes[c];
        fprintf(file,"  C%d[\"", c+1);
        for (int k = 0; k < cls->nb_vertices; ++k) {
            if (k) printf(", ");
            fprintf(file,"%d", cls->vertices[k]);
        }
        fprintf(file, "\"]\n");
    }
    for (int i = 0; i < L->size; ++i) {
        fprintf(file,"  C%d --> C%d\n", L->links[i].start + 1, L->links[i].end + 1);
    }

    fclose(file);
    printf("Mermaid file '%s' generated successfully.\n", filename);
}

void removeTransitiveLinks(t_link_array *p_link_array)
{
    int i = 0;
    while (i < p_link_array->size)
    {
        t_link link1 = p_link_array->links[i];
        int j = 0;
        int to_remove = 0;
        while (j < p_link_array->size && !to_remove)
        {
            if (j != i)
            {
                t_link link2 = p_link_array->links[j];
                if (link1.start == link2.start)
                {
                    // look for a link from link2.end to link1.end
                    int k = 0;
                    while (k < p_link_array->size && !to_remove)
                    {
                        if (k != j && k != i)
                        {
                            t_link link3 = p_link_array->links[k];
                            // Fixed: check link3.end == link1.end (not link3.start)
                            if ((link3.start == link2.end) && (link3.end == link1.end))
                            {
                                to_remove = 1;
                            }
                        }
                        k++;
                    }
                }
            }
            j++;
        }
        if (to_remove)
        {
            // remove link1 by replacing it with the last link
            p_link_array->links[i] = p_link_array->links[p_link_array->size - 1];
            p_link_array->size--;
        }
        else
        {
            i++;
        }
    }
}

int is_class_transient(int class_id, t_link_array *class_links) {
    for (int i = 0; i < class_links->size; i++) {
        if (class_links->links[i].start == class_id &&
            class_links->links[i].end != class_id) {
            return 1;
            }
    }
    return 0;
}

int is_class_persistent(int class_id, t_link_array *class_links) {
    if (is_class_transient(class_id, class_links) == 1) {
        return 0;
    } else {
        return 1;
    }
}

int is_state_absorbing(int class_id, t_partition *partition, t_link_array *class_links) {
    if (is_class_transient(class_id, class_links)) {
        return 0;
    }

    if (partition->classes[class_id]->nb_vertices == 1) {
        return 1;
    }

    return 0;
}

int is_graph_irreducible(t_partition *partition) {
    if (partition == NULL) {
        printf("Error: NULL partition provided\n");
        return 0;
    }

    const int number_of_classes = partition->nb_class;

    if (number_of_classes == 1) {
        return 1;
    } else {
        return 0;
    }
}

/**
 * Display all graph characteristics with class numbers
 */
void display_graph_characteristics(t_partition *partition, t_link_array *class_links) {
    printf("For classes:\n");
    for (int i = 0; i < partition->nb_class; i++) {
        t_class *current_class = partition->classes[i];

        printf("  Class %s (Class #%d) {", current_class->name, i + 1);
        for (int j = 0; j < current_class->nb_vertices; j++) {
            if (j > 0) printf(", ");
            printf("%d", current_class->vertices[j]);
        }
        printf("}: ");

        if (is_class_transient(i, class_links)) {
            printf("transient\n");
        } else {
            printf("persistent\n");
        }
    }

    printf("\nFor states (transient or absorbing):\n");
    for (int i = 0; i < partition->nb_class; i++) {
        t_class *current_class = partition->classes[i];

        for (int j = 0; j < current_class->nb_vertices; j++) {
            int vertex = current_class->vertices[j];
            printf("  State %d (in %s, Class #%d): ", vertex, current_class->name, i + 1);

            if (is_class_transient(i, class_links)) {
                printf("transient\n");
            } else {
                printf("persistent");
                if (is_state_absorbing(i, partition, class_links)) {
                    printf(", absorbing\n");
                } else {
                    printf("\n");
                }
            }
        }
    }

    printf("\nFor states (absorbing):\n");
    int has_absorbing = 0;
    for (int i = 0; i < partition->nb_class; i++) {
        if (is_state_absorbing(i, partition, class_links)) {
            t_class *current_class = partition->classes[i];
            printf("  State %d (in %s, Class #%d) is absorbing\n",
                   current_class->vertices[0], current_class->name, i + 1);
            has_absorbing = 1;
        }
    }
    if (!has_absorbing) {
        printf("  No absorbing states\n");
    }

    printf("\nIs the graph reductible:\n");
    if (is_graph_irreducible(partition)) {
        printf("  The Markov graph is irreducible\n");
    } else {
        printf("  The Markov graph is reducible\n");
    }

    printf("\n");
}
