#include "hasse.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * @brief Initializes a t_link_array structure.
 * @param a Pointer to the link array to initialize
 */
void links_init(t_link_array *a) {
    a->links = NULL;
    a->size = 0;
    a->length = 0;
}

/**
 * @brief Adds a link (start → end) to the array unless it already exists.
 *        Grows the array dynamically when needed.
 * @param a Pointer to the link array
 * @param start Origin class index
 * @param end Destination class index
 */
void links_add(t_link_array *a, int start, int end) {
    // Prevent duplicate links
    for (int i = 0; i < a->size; ++i) {
        if (a->links[i].start == start && a->links[i].end == end) return;
    }

    // Resize array if needed
    if (a->size == a->length) {
        a->length = (a->length == 0) ? 8 : a->length * 2;
        a->links = (t_link*)realloc(a->links, a->length * sizeof *a->links);
    }

    // Insert new link
    a->links[a->size++] = (t_link){ start, end };
}

/**
 * @brief Creates an array mapping each vertex to its class number.
 * @param vertex_count Total number of vertices
 * @param partition Pointer to the partition structure
 * @return Newly allocated array of size vertex_count, NULL on error
 */
int *create_array_vertex_to_class(int vertex_count, const t_partition *partition) {
    int *vertex_to_class = (int*)malloc(vertex_count * sizeof *vertex_to_class);
    if (!vertex_to_class) return NULL;

    // Initialize all vertices as "unassigned"
    for (int vertex = 0; vertex < vertex_count; ++vertex)
        vertex_to_class[vertex] = -1;

    // Assign class index to each vertex in each class
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

/**
 * @brief Computes class‑to‑class links from a vertex adjacency list.
 * @param graph Pointer to the adjacency list
 * @param vertex_to_class Mapping from vertex to class
 * @param class_links Output: list of links between classes
 */
void list_class_links(const t_adjacency_list *graph,
                      const int *vertex_to_class,
                      t_link_array *class_links)
{
    for (int source_vertex = 0; source_vertex < graph->size; ++source_vertex) {
        int source_class = vertex_to_class[source_vertex];

        p_cell neighbor_cell = graph->array[source_vertex].head;
        while (neighbor_cell != NULL) {
            int neighbor_id_1based = neighbor_cell->arrival;
            int neighbor_index = neighbor_id_1based - 1;

            if (neighbor_index >= 0 && neighbor_index < graph->size) {
                int destination_class = vertex_to_class[neighbor_index];

                // Record inter‑class link only
                if (source_class != destination_class) {
                    links_add(class_links, source_class, destination_class);
                }
            }

            neighbor_cell = neighbor_cell->next;
        }
    }
}

/**
 * @brief Writes a Mermaid Hasse diagram to a file.
 * @param P Pointer to the partition structure
 * @param L Pointer to the list of class links
 * @param filename Output file path
 */
void print_hasse_mermaid(const t_partition *P, const t_link_array *L, const char *filename)
{
    FILE *file = fopen(filename, "wt");
    if (file == NULL) {
        perror("Could not open file for writing");
        exit(EXIT_FAILURE);
    }

    // Mermaid front‑matter
    fprintf(file, "---\n");
    fprintf(file, "config:\n");
    fprintf(file, "   layout: elk\n");
    fprintf(file, "   theme: neo\n");
    fprintf(file, "   look: neo\n");
    fprintf(file, "---\n\n");

    fprintf(file,"flowchart TD\n");

    // Print classes
    for (int c = 0; c < P->nb_class; ++c) {
        const t_class *cls = P->classes[c];
        fprintf(file,"  C%d[\"", c+1);
        for (int k = 0; k < cls->nb_vertices; ++k) {
            if (k) fprintf(file,", ");
            fprintf(file,"%d", cls->vertices[k]);
        }
        fprintf(file, "\"]\n");
    }

    // Print links
    for (int i = 0; i < L->size; ++i) {
        fprintf(file,"  C%d --> C%d\n", L->links[i].start + 1, L->links[i].end + 1);
    }

    fclose(file);
    printf("Mermaid file '%s' generated successfully.\n", filename);
}

/**
 * @brief Removes transitively implied links (computes transitive reduction).
 * @param p_link_array Pointer to the link array to update
 */
void removeTransitiveLinks(t_link_array *p_link_array)
{
    int i = 0;
    while (i < p_link_array->size)
    {
        t_link link1 = p_link_array->links[i];
        int to_remove = 0;

        // Look for link1 = (A → C) being implied by A → B and B → C
        for (int j = 0; j < p_link_array->size && !to_remove; j++) {
            if (j == i) continue;

            t_link link2 = p_link_array->links[j];

            if (link1.start == link2.start) {
                for (int k = 0; k < p_link_array->size && !to_remove; k++) {
                    if (k == j || k == i) continue;

                    t_link link3 = p_link_array->links[k];

                    if (link3.start == link2.end && link3.end == link1.end)
                        to_remove = 1;
                }
            }
        }

        if (to_remove) {
            // Remove link by replacing with last element
            p_link_array->links[i] = p_link_array->links[p_link_array->size - 1];
            p_link_array->size--;
        } else {
            i++;
        }
    }
}

/**
 * @brief Determines whether a class is transient.
 * @param class_id Class index
 * @param class_links Pointer to the class link array
 * @return 1 if transient, 0 otherwise
 */
int is_class_transient(int class_id, t_link_array *class_links) {
    for (int i = 0; i < class_links->size; i++) {
        if (class_links->links[i].start == class_id &&
            class_links->links[i].end != class_id) {
            return 1;
        }
    }
    return 0;
}

/**
 * @brief Determines whether a class is persistent (non‑transient).
 * @param class_id Class index
 * @param class_links Pointer to link array
 * @return 1 if persistent, 0 if transient
 */
int is_class_persistent(int class_id, t_link_array *class_links) {
    return !is_class_transient(class_id, class_links);
}

/**
 * @brief Determines if a class corresponds to an absorbing state.
 *        A class is absorbing if:
 *          - it is persistent
 *          - it contains exactly one vertex
 * @param class_id Class index
 * @param partition Pointer to the partition
 * @param class_links Pointer to class links
 * @return 1 if absorbing, 0 otherwise
 */
int is_state_absorbing(int class_id, t_partition *partition, t_link_array *class_links) {
    if (is_class_transient(class_id, class_links)) {
        return 0;
    }

    // Single‑vertex persistent class = absorbing
    if (partition->classes[class_id]->nb_vertices == 1) {
        return 1;
    }

    return 0;
}

/**
 * @brief Checks if the partition graph is irreducible.
 *        A Markov graph is irreducible if it has exactly one class.
 * @param partition Pointer to the partition
 * @return 1 if irreducible, 0 otherwise
 */
int is_graph_irreducible(t_partition *partition) {
    if (partition == NULL) {
        printf("Error: NULL partition provided\n");
        return 0;
    }

    return (partition->nb_class == 1);
}

/**
 * @brief Displays all graph characteristics: class types, state types,
 *        absorbing states, and overall reducibility.
 * @param partition Pointer to the partition
 * @param class_links Pointer to class‑to‑class link array
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

        printf(is_class_transient(i, class_links) ? "transient\n" : "persistent\n");
    }

    printf("\nFor states (transient or absorbing):\n");
    for (int i = 0; i < partition->nb_class; i++) {
        t_class *current_class = partition->classes[i];

        for (int j = 0; j < current_class->nb_vertices; j++) {
            int vertex = current_class->vertices[j];
            printf("  State %d (in %s, Class #%d): ",
                   vertex, current_class->name, i + 1);

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
