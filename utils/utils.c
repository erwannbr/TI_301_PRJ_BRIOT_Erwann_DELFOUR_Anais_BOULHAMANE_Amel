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

void PowerMatrix (p_matrix M, int power) {
    for (int i=0; i<power-1; i++) {
        M = MultiplyMatrices(M, M);
    }
    printf("Meteo Matrix Power %d\n", power);
    printMatrix(M);
}

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

//we use lazy chain because otherwise we wille never get an answer otherwise (thanks chat gpt because it is impossible to find this in other ways)
//we create an identity matrix (for the lazy work)
p_matrix CreateIdentityMatrix(int n) {
    p_matrix I = CreateEmptyMatrix(n);
    for (int i = 0; i < n; i++) {
        I->data[i][i] = 1.0f;
    }
    return I;
}

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

//calculates stationary distribution for a specific class matrix. In other word we forec the matrix to settle into its natural equilibrium (which is called the stationnary distribution)
void SolveStationaryDistribution(p_matrix M, int period) {
    int n = M->size;
    p_matrix WorkingMatrix;

    if (period > 1) {
        printf("      Class is periodic (d=%d). Using Lazy Walk (0.5*M + 0.5*I).\n", period);
        p_matrix I = CreateIdentityMatrix(n);
        WorkingMatrix = MixMatrices(M, I, 0.5f);
        DestroyMatrix(I);
    } else {
        //if aperiodic
        WorkingMatrix = CreateEmptyMatrix(n);
        CopyMatrix(WorkingMatrix, M);
    }

    //we search the end of the limits
    p_matrix Current = CreateEmptyMatrix(n);
    CopyMatrix(Current, WorkingMatrix);

    p_matrix Next;
    float diff = 1.0f;
    int iter = 0;
    float epsilon = 0.0001f;

    while (diff > epsilon && iter < 10000) {
        Next = MultiplyMatrices(Current, WorkingMatrix);
        diff = DiffMatrix(Current, Next);

        DestroyMatrix(Current);
        Current = Next;
        iter++;
    }

    printf("      Converged in %d iterations.\n", iter);
    printf("      Stationary Distribution: [ ");
    for (int j = 0; j < n; j++) {
        printf("%.4f ", Current->data[0][j]);
    }
    printf("]\n");

    //free needed to dont have leak of memory
    DestroyMatrix(WorkingMatrix);
    DestroyMatrix(Current);
}

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
        p_matrix subM = SubMatrixByComponent(FullMatrix, *partition, i);

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