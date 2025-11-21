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
int *create_vertex_class_array(int vertex_count, const t_partition *partition) {
    // vertex_class[v] will store the index of the class that vertex v belongs to.
    int *vertex_class = (int*)malloc(vertex_count * sizeof *vertex_class);
    if (!vertex_class) return NULL;

    // Here, -1 means: this vertex has not been assigned to any class yet.
    for (int vertex = 0; vertex < vertex_count; ++vertex)
        vertex_class[vertex] = -1;

    // for each class in the partition
    for (int class_index = 0; class_index < partition->nb_class; ++class_index) {
        // Get a pointer to the current class
        t_class *cls = partition->classes[class_index];
        // First loop over the vertices of this class
        for (int i = 0; i < cls->nb_vertices; ++i) {
            int vertex_id = cls->vertices[i];   // Vertex ID in the graph
            int vertex_index = vertex_id - 1;
            if (vertex_index >= 0 && vertex_index < vertex_count)
                vertex_class[vertex_index] = class_index;
        }
    }
    return vertex_class;
}

/**
 * @brief Computes class‑to‑class links from a vertex adjacency list.
 * @param graph Pointer to the adjacency list
 * @param vertex_to_class Mapping from vertex to class
 * @param class_links Output: list of links between classes
 */
void list_class_links(const t_adjacency_list *graph, const int *vertex_to_class, t_link_array *class_links) {
    // For each vertex i in the graph
    for (int i= 0; i < graph->size; ++i) {
        // Ci = class of vertex i
        int Ci = vertex_to_class[i];
        // Access the adjacency list of vertex i
        p_cell neighbor = graph->array[i].head;
        // Traverse all neighbors j of vertex i
        while (neighbor != NULL) {
            int j_id= neighbor->arrival; // Neighbor vertex ID
            int j = j_id - 1; // Convert to 0-based index

            if (j >= 0 && j < graph->size) {
                // Cj = class of neighbor j
                int Cj = vertex_to_class[j];

                // Only record links between *different* classes
                if (Ci != Cj) {
                    // Add the link (Ci → Cj) unless it already exists.
                    // links_add() handles duplicate prevention and resizing.
                    links_add(class_links, Ci, Cj);
                }
            }
            // Move to the next neighbor in the adjacency list
            neighbor = neighbor->next;
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

    // Iterate over each candidate link (A -> C)
    while (i < p_link_array->size) {
        t_link ac = p_link_array->links[i];  // candidate link: A -> C
        int to_remove = 0;
        // Look for an intermediate link A -> B
        for (int j = 0; j < p_link_array->size && !to_remove; j++) {
            if (j == i) continue; // skip the same link
            t_link ab = p_link_array->links[j]; // potential A -> B
            // We only care about links that start from the same A
            if (ab.start != ac.start) continue;
            // Now look for a link B -> C
            for (int k = 0; k < p_link_array->size && !to_remove; k++) {
                if (k == i || k == j) continue;
                t_link bc = p_link_array->links[k]; // potential B -> C
                // If we have A -> B and B -> C, then A -> C is transitive
                if (bc.start == ab.end && bc.end == ac.end) {
                    to_remove = 1;
                }
            }
        }
        if (to_remove) {
            // Remove the transitive link A -> C by replacing it with the last link
            p_link_array->links[i] = p_link_array->links[p_link_array->size - 1];
            p_link_array->size--;
            // Do not increment i here: we need to re-check the new element at index i
        } else {
            // Keep this link and move to the next one
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
