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
 * following lines = "start end probability"
 * @note Exits program on file error
 */
t_adjacency_list readGraph(const char *filename) {
    // open the file in text mode for reading ("rt")
    FILE *file = fopen(filename, "rt");
    if (!file) {
        // if the file could not be opened, print an error message and stop the program
        perror("Could not open file for reading");
        exit(EXIT_FAILURE);
    }

    int nb_vertices, start, end;
    float proba;
    // read the number of vertices (first integer in the file) (fscanf sent 0 or 1)
    if (fscanf(file, "%d", &nb_vertices) != 1) {
        // if we cannot read it, print an error and exit
        perror("Could not read number of vertices");
        fclose(file);
        exit(EXIT_FAILURE);
    }
    // create an empty adjacency list with nb_vertices vertices
    t_adjacency_list *gp = empty_adjacency_list(nb_vertices);
    // read each line: start vertex, end vertex, and probability
    // Continue while fscanf successfully reads 3 values
    while (fscanf(file, "%d %d %f", &start, &end, &proba) == 3) {
        // add an edge from 'start' to 'end' with probability 'proba'
        // we use start - 1 because array indices go from 0 to nb_vertices - 1
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
//
/**
 * @brief Checks if the graph satisfies Markov chain properties
 * @param list Adjacency list to check
 * @note Verifies that outgoing probabilities from each vertex sum to 1.0
 * @note Tolerance: ±0.01 for floating-point comparison
 */
void checkIfMarkov(t_adjacency_list list) {
    int is_markov = 1; // it indicates if the graph satisfies the Markov property

    // loop through each vertex in the graph
    for (int i = 0; i < list.size; i++) {
        float total_proba = 0;
        t_cell *curr = list.array[i].head;

        // sum all probabilities for the current vertex
        while (curr != NULL) {
            total_proba += curr->probability;
            curr = curr->next;
        }

        // sum all outgoing probabilities for the current vertex
        if (total_proba < 0.99 || total_proba > 1) {
            is_markov = 0;

            // print detailed error
            printf( "Vertex %d does not respect the Markov property: sum is %.2f\n", i + 1, total_proba );
            break; // stop at the first error
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
        printf("Error: cannot open file '%s'\n", filename);
        return;    // failure
    }

    // write the Mermaid configuration header.
    // these settings control the appearance of the graph in MermaidChart.
    fprintf(file, "---\n");
    fprintf(file, "config:\n");
    fprintf(file, "   layout: elk\n");
    fprintf(file, "   theme: neo\n");
    fprintf(file, "   look: neo\n");
    fprintf(file, "---\n\n");

    // start a flowchart that goes from left to right.
    fprintf(file, "flowchart LR\n");

    // write each vertex definition.
    // example: A((1)), B((2)), C((3)), etc.
    // getID(i + 1) converts 1 → A, 2 → B, ..., 27 → AA.
    for (int i = 0; i < graph.size; i++) {
        fprintf(file, "%s((%d))\n", getID(i + 1), i + 1);
    }
    fprintf(file, "\n");
    // write all edges based on the adjacency list.
    // for each vertex i, traverse its linked list of outgoing edges.
    for (int i = 0; i < graph.size; i++) {
        t_cell *current = graph.array[i].head;
        // for every outgoing edge: print
        // source -->|proba| destination
        while (current != NULL) {
            fprintf(file, "%s -->|%.2f|%s\n",getID(i + 1), current->probability,  getID(current->arrival)); // Convert source vertex index to letter,Display probability,Convert destination vertex number to letter
            current = current->next; // Move to the next outgoing edge
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

/**
 * @brief Prints links between classes before and after removing transitive edges.
 *
 * @param partition The partition of the graph.
 * @param class_links Structure containing all links between classes.
 */
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

/**
 * @brief Prints the contents (vertices) of each component in the partition.
 *
 * @param partition The partition structure containing the components.
 */
void print_component(p_partition partition) {
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
}

/**
 * @brief Raises a matrix to a specified power and prints it.
 *
 * @param M The base matrix.
 * @param power The exponent.
 */
void PowerMatrix (p_matrix M, int power) {
    for (int i=0; i<power-1; i++) {
        M = MultiplyMatrices(M, M);
    }
    printf("Meteo Matrix Power %d\n", power);
    printMatrix(M);
}

/**
 * @brief Computes and prints the stationary matrix by repeated squaring until convergence.
 *
 * Checks for convergence using the difference between successive powers (M^2^k).
 *
 * @param graph The source graph.
 * @param epsilon The convergence threshold.
 * @param graph_name Name of the graph for display purposes.
 */
void ComputeStationaryMatrix (t_adjacency_list graph, float epsilon, const char *graph_name) {
    p_matrix M = CreateMatFromAdjList(graph);
    p_matrix MNext = MultiplyMatrices(M, M);
    float diff = DiffMatrix(M, MNext);
    int power = 1;

    while (diff > epsilon && power < 100) {
        power ++;
        p_matrix tmp = MultiplyMatrices(MNext, M);
        diff = DiffMatrix(MNext, tmp);

        printf("For iteration %d, the difference is %.5f\n", power, diff);

        free(M);
        M = MNext;
        MNext = tmp;

    }
    if (power > 100) printf ("For %s, this criterion does not work\n", graph_name);
    else {
        printf ("Convergence reached afeter %d multiplications (diff = %.5f)\n", power, diff);
        printf("Stationary Matrix :\n");
        printMatrix(MNext);
    }
    free(M);
    free(MNext);

}

/**
 * @brief Creates an identity matrix of size n x n.
 *
 * @param n The dimension of the matrix.
 * @return p_matrix The resulting identity matrix.
 */
//we use lazy chain because otherwise we wille never get an answer otherwise (thanks chat gpt because it is impossible to find this in other ways)
//we create an identity matrix (for the lazy work)
p_matrix CreateIdentityMatrix(int n) {
    p_matrix I = CreateEmptyMatrix(n);
    for (int i = 0; i < n; i++) {
        I->data[i][i] = 1.0f;
    }
    return I;
}

/**
 * @brief Computes a linear combination of two matrices (alpha*A + (1-alpha)*B).
 *
 * Used to create a "Lazy Walk" matrix to solve periodic chains.
 *
 * @param A First matrix.
 * @param B Second matrix.
 * @param alpha Weight for the first matrix.
 * @return p_matrix The resulting mixed matrix.
 */
//mix matrices (mathematical expression: 0.5*M + 0.5*I) because other wise it will cycle forever (ping-pong (A= 1, B= 0->A=0, B=1->etc) so with alpha = 0.5
p_matrix MixMatrices(p_matrix A, p_matrix B, float alpha) {
    if (A->size != B->size) return NULL;
    int n = A->size;
    p_matrix R = CreateEmptyMatrix(n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            R->data[i][j] = alpha * A->data[i][j] + (1.0f - alpha) * B->data[i][j];
        }
    }
    return R;
}

/**
 * @brief Computes and prints the stationary distribution vector for a matrix.
 *
 * Handles periodic matrices by applying a "Lazy Walk" transformation before solving.
 *
 * @param M The transition matrix.
 * @param period The period of the matrix (1 if aperiodic).
 */
//calculates stationary distribution for a specific class matrix. In other word we forec the matrix to settle into its natural equilibrium (which is called the stationnary distribution)
void SolveStationaryDistribution(p_matrix M, int period) {
    p_matrix MatrixToSolve;
    p_matrix I = NULL;
    p_matrix Lazy = NULL;

    if (period > 1) {
        printf("      [Periodic d=%d] Using Lazy Walk (0.5M + 0.5I).\n", period);
        I = CreateIdentityMatrix(M->size);
        Lazy = MixMatrices(M, I, 0.5f);
        MatrixToSolve = Lazy;
    } else {
        MatrixToSolve = M;
    }

    float *pi = StationaryVectorFromSubmatrix(MatrixToSolve, 10000, 1e-6f);

    if (pi) {
        printf("   Stationary Distribution: [ ");
        for (int j = 0; j < M->size; ++j) printf("%.4f ", pi[j]);
        printf("]\n");
        free(pi);
    } else {
        printf("      [Error] Did not converge.\n");
    }

    if (I) DestroyMatrix(I);
    if (Lazy) DestroyMatrix(Lazy);
}

/**
 * @brief Analyzes periodicity and stationary distribution for all components of a graph.
 *
 * Decomposes the graph into SCCs, calculates the period for each, and solves for the stationary distribution.
 *
 * @param graph The graph to analyze.
 */
void periodicity(t_adjacency_list graph) {
    //get partition using tarjan
    p_partition partition = tarjan(graph);
    p_matrix FullMatrix = CreateMatFromAdjList(graph);

    //iterate over each classes found
    for (int i = 0; i < partition->nb_class; i++) {
        t_class *cls = partition->classes[i];

        printf("\nAnalyzing Class #%d (Vertices: ", i + 1);
        for(int v=0; v < cls->nb_vertices; v++) printf("%d ", cls->vertices[v]);
        printf("):\n");

        //extract submatrix
        p_matrix subM = subMatrix(FullMatrix, *partition, i);

        if (subM == NULL) {
            printf("   Could not extract matrix (likely size 0 or invalid index).\n");
            continue;
        }

        //calculate the period
        int period = getPeriod(subM);
        printf("   Period: %d\n", period);

        // calculate the stationnary distribution
        SolveStationaryDistribution(subM, period);

        DestroyMatrix(subM);
    }

    DestroyMatrix(FullMatrix);
}

/**
 * @brief Performs the second step of validation: classification and distribution analysis.
 *
 * Identifies persistent vs transient classes and computes stationary distributions for persistent ones.
 *
 * @param graph The graph to analyze.
 */
void step2_validation(t_adjacency_list graph) {
    p_matrix M = CreateMatFromAdjList(graph);

    p_partition P = tarjan(graph);

    int *map = create_vertex_class_array(graph.size, P);
    t_link_array L;
    links_init(&L);
    list_class_links(&graph, map, &L);

    const int MAX_IT = 1000;
    const float EPS = 1e-6f;

    for (int c = 0; c < P->nb_class; ++c) {
        p_class cls = P->classes[c];

        // Check if the class is persistent (no edges leaving the class)
        int persistent = is_class_persistent(c, &L);

        // Print Class Name and Vertices
        printf("Classe C%d (", c + 1);
        for (int k = 0; k < cls->nb_vertices; ++k) {
            if (k > 0) printf(", ");
            printf("%d", cls->vertices[k]);
        }
        printf(") : %s\n", persistent ? "Persistent" : "Transient");

        // Extract the sub-matrix for this specific class
        p_matrix S = subMatrix(M, *P, c);

        if (!S) {
            printf("  (Sub-matrices can't found)\n");
            continue;
        }

        if (!persistent) {
            // Transient classes eventually have 0 probability in the long run
            printf("  Distribution limit: [ ");
            for (int j = 0; j < S->size; ++j) printf("0.0000 ");
            printf("]\n");
        } else {
            // Persistent classes have a stationary distribution (Equilibrium)
            float *pi = StationaryVectorFromSubmatrix(S, MAX_IT, EPS);
            if (!pi) {
                printf("  Error on computing\n");
            } else {
                printf("   Stationary Distribution: [ ");
                for (int j = 0; j < S->size; ++j) {
                    printf("%.4f ", pi[j]);
                }
                printf("]\n");
                free(pi); // Don't forget to free the vector
            }
        }

        DestroyMatrix(S);
    }

    DestroyMatrix(M);
    if (map) free(map);
    if (L.links) free(L.links);
}