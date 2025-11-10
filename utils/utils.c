#include <stdio.h>
#include <stdlib.h>

#include "../utils/utils.h"
#include "../adjacency_list/adjacency_list.h"
#include "../hasse/hasse.h"
#include "../tarjan/tarjan.h"

/**
 * @brief Reads a graph from a file and creates its adjacency list
 * @param filename Path to the file containing graph data
 * @return Adjacency list representation of the graph
 * @note File format: first line = number of vertices,
 *       following lines = "start end probability"
 * @note Exits program on file error
 */
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


/**
 * @brief Converts vertex index to alphabetic ID (1->A, 27->AA, etc.)
 * @param i Vertex index (1-based)
 * @return Static string containing the alphabetic ID
 * @note Uses rotating buffer to allow multiple calls in same expression
 * @warning Returned pointer is static and will be overwritten after 20 calls
 */
static char *getID(int i)
{
    static char buffer[20][10];  // Array of buffers
    static int idx = 0;
    idx = (idx + 1) % 20;        // Rotate through them

    char *result = buffer[idx];
    char temp[10];
    int index = 0;

    i--;
    while (i >= 0)
    {
        temp[index++] = 'A' + (i % 26);
        i = (i / 26) - 1;
    }

    for (int j = 0; j < index; j++)
    {
        result[j] = temp[index - j - 1];
    }
    result[index] = '\0';

    return result;
}


/**
 * @brief Checks if the graph satisfies Markov chain properties
 * @param list Adjacency list to check
 * @note Verifies that outgoing probabilities from each vertex sum to 1.0
 * @note Tolerance: ±0.01 for floating-point comparison
 */
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


/**
 * @brief Exports the graph to Mermaid diagram format
 * @param graph Adjacency list to export
 * @param filename Output file path
 * @note Creates a flowchart with ELK layout and neo theme
 * @note Exits program on file error
 */
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
            fprintf(file, "%s -->|%.2f|%s\n",getID(i + 1), current->probability,  getID(current->arrival));
            current = current->next;
        }
    }

    fclose(file);
    printf("Mermaid file '%s' generated successfully.\n", filename);
}

/**
 * @brief Converts adjacency list to Tarjan vertex array
 * @param graph Pointer to adjacency list
 * @return Array of Tarjan vertices for SCC algorithm
 */
t_tarjan_vertex * GraphIntoTar (t_adjacency_list* graph) {
    t_tarjan_vertex * vertices = CreateArr(graph->size);
    return vertices;
}

/**
 * @brief Creates an empty stack
 * @return Pointer to new stack
 */
t_stack * CreateStack () {
    t_stack * s = malloc (sizeof(t_stack));
    s->head = NULL;
    return s;
}

/**
 * @brief Pushes a vertex ID onto the stack
 * @param s Pointer to stack
 * @param vertex_id ID of vertex to push
 */
void push (t_stack *s, int vertex_id) {
    t_stack_node * new = malloc (sizeof(t_stack_node));
    new->vertex_id = vertex_id;
    new->next = s->head;
    s->head = new;
}

/**
 * @brief Pops a vertex ID from the stack
 * @param s Pointer to stack
 * @return Vertex ID, or -1 if stack is empty
 */
int pop (t_stack *s) {
    if (s->head ==NULL) return -1;
    int val = s->head->vertex_id;
    t_stack_node *tmp = s->head;
    s->head= s->head->next;
    free(tmp);
    return val;
}

/**
 * @brief Checks if stack is empty
 * @param s Pointer to stack
 * @return 1 if empty, 0 otherwise
 */
int isEmpty (t_stack *s) {
    return (s->head == NULL);
}

void print_class(p_partition partition,  t_link_array class_links) {
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
}
