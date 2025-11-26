#include "matrix.h"
#include "../cell/cell.h"
#include "../utils/utils.h"
#include <stdio.h>
#include <stdlib.h>


/**
 * @brief Allocates and initializes a square matrix of size n x n with zeros.
 *
 * @param n The dimension of the matrix.
 * @return p_matrix Pointer to the newly allocated matrix.
 */
p_matrix CreateEmptyMatrix(int n) {
    // Allocate memory for the matrix structure
    p_matrix M = (p_matrix)malloc(sizeof(t_matrix));
    if (!M) {
        // Allocation failed
        return NULL;
    }

    // Store the dimension of the matrix (n x n)
    M->size = n;

    // Allocate an array of n pointers, one pointer for each row
    M->data = malloc(n * sizeof(float *));

    // For each row, allocate n floats and initialize them to 0 using calloc
    for (int i = 0; i < n; i++) {
        M->data[i] = calloc(n, sizeof(float));
        // calloc sets all entries in the row to 0.0
    }

    return M;
}


/**
 * @brief Converts an adjacency list graph into a transition matrix (probability matrix).
 *
 * Each entry M[i][j] represents the probability of moving from node i to node j.
 *
 * @param graph The source adjacency list.
 * @return p_matrix Pointer to the resulting matrix.
 */
p_matrix CreateMatFromAdjList(t_adjacency_list graph) {
    int n = graph.size;
    // Create an empty n×n matrix initialized with zeros
    p_matrix M = CreateEmptyMatrix(n);
    // For each vertex i in the graph
    for (int i = 0; i < n; i++) {
        // Pointer to the adjacency list of vertex i
        t_cell *neigh = graph.array[i].head;
        // Traverse all outgoing edges from vertex i
        while (neigh != NULL) {
            // j = index of the destination vertex
            int j = neigh->arrival - 1;
            // Fill the matrix entry M[i][j] with the transition probability
            M->data[i][j] = neigh->probability;
            // Move to next neighbor in the adjacency list
            neigh = neigh->next;
        }
    }

    return M;
}

/**
 * @brief Copies the content of a source matrix into a destination matrix.
 *
 * @param mat The destination matrix (must be allocated and of same size).
 * @param matsrc The source matrix.
 */

void CopyMatrix(p_matrix mat, p_matrix matsrc) {
    // Check that both matrices have the same size (n x n)
    if (mat->size != matsrc->size) {
        // Sizes differ: do nothing and return.
        return;
    }

    // Copy each element from matsrc into mat
    for (int i = 0; i < matsrc->size; i++) {
        for (int j = 0; j < matsrc->size; j++) {
            mat->data[i][j] = matsrc->data[i][j];
        }
    }
}

/**
 * @brief Performs matrix multiplication (A * B).
 *
 * @param A The first matrix.
 * @param B The second matrix.
 * @return p_matrix A new matrix containing the result.
 */

p_matrix MultiplyMatrices(p_matrix A, p_matrix B) {
    // Check that the two matrices are compatible for multiplication.
    if (A->size != B->size) {
        // Incompatible sizes
        return NULL;
    }
    int n = A->size;
    // Create an empty n x n result matrix, initialized with 0
    p_matrix R = CreateEmptyMatrix(n);

    // Standard matrix multiplication: R = A * B
    for (int i = 0; i < n; i++) {          // Row index of A and R
        for (int j = 0; j < n; j++) {      // Column index of B and R
            float sum = 0.0f;

            // Compute the dot product of row i of A and column j of B
            for (int k = 0; k < n; k++) {
                sum += A->data[i][k] * B->data[k][j];
            }

            R->data[i][j] = sum;
        }
    }

    return R;  // Return the resulting matrix
}

/**
 * @brief Calculates the sum of absolute differences between two matrices.
 *
 * Used to check for convergence (e.g., if M and N are very close).
 *
 * @param M The first matrix.
 * @param N The second matrix.
 * @return float The total accumulated difference.
 */

float DiffMatrix(p_matrix M, p_matrix N) {
    // Check that both matrices have the same size
    if (M->size != N->size) {
        // Different sizes: return a negative value to signal an error
        return -1.0f;
    }

    float diff;
    float sum = 0.0f;

    // Compute the sum of absolute differences between all entries
    for (int i = 0; i < M->size; i++) {
        for (int j = 0; j < M->size; j++) {
            diff = M->data[i][j] - N->data[i][j];

            // Take absolute value of the difference
            if (diff < 0.0f) {
                diff = -diff;
            }

            // Accumulate the absolute difference
            sum += diff;
        }
    }

    // Return the total difference between the two matrices
    return sum;
}


/**
 * @brief Prints the matrix values to the standard output.
 *
 * @param M The matrix to display.
 */
void printMatrix(p_matrix M) {
    int n = M->size;
    for (int i = 0; i<n ; i++) {
        printf("[");
        for (int j = 0; j<n ; j++) {
            printf("%.2f  ",M->data[i][j]);
        }
        printf("]\n");
    }
    printf("\n");
    printf("\n");
}

/**
 * @brief Extracts a submatrix corresponding to a specific component
 *        (class) of the graph partition.
 *
 * The idea:
 * - 'matrix' is the full transition matrix of the Markov graph (n x n).
 * - 'part' is the partition into strongly connected components (classes).
 * - 'compo_index' selects one class (component) inside this partition.
 *
 * This function builds a new matrix that only contains the rows and columns
 * of the vertices that belong to this selected class.
 *
 * Example:
 *   - Suppose matrix is 6x6 (vertices 1..6).
 *   - Component C2 = {2, 5, 6}.
 *   - Then the submatrix for C2 is the 3x3 matrix whose entries are:
 *         [ M(2,2)  M(2,5)  M(2,6) ]
 *         [ M(5,2)  M(5,5)  M(5,6) ]
 *         [ M(6,2)  M(6,5)  M(6,6) ]
 */

p_matrix subMatrix(p_matrix M, t_partition part, int compo_index)
{
    if (!M) return NULL; // Check that the full matrix exists
    if (compo_index < 0 || compo_index >= part.nb_class) return NULL; // Check that the class index is valid

    t_class *cls = part.classes[compo_index]; // Get the class corresponding to the index
    int n = cls->nb_vertices; // Number of vertices inside this class
    if (n <= 0) return NULL; // A class with no vertices → no submatrix to build


    p_matrix sub = CreateEmptyMatrix(n); // Create an empty n×n matrix where we will store the submatrix
    if (!sub) return NULL;

    // Copy each row and column from M corresponding to the vertices of the class
    for (int i = 0; i < n; i++) {
        int row = cls->vertices[i] - 1;''
        for (int j = 0; j < n; j++) {
            int col = cls->vertices[j] - 1;
            sub->data[i][j] = M->data[row][col]; // Copy the value from the full matrix into the submatrix
        }
    }

    return sub;
}

/**
 * @brief Computes the stationary probability vector (Pi) for a submatrix.
 *
 * Uses the iterative power method until convergence or max iterations is reached.
 *
 * @param S The submatrix (transition matrix of an SCC).
 * @param max_iter Maximum number of iterations.
 * @param eps Convergence threshold (epsilon).
 * @return float* A dynamically allocated array representing the stationary vector.
 */
float *StationaryVectorFromSubmatrix(p_matrix S, int max_iter, float eps) {
    if (!S || S->size <= 0 || max_iter <= 0 || eps <= 0.0f) return NULL;
    int n = S->size;

    if (n == 1) {
        float *pi = (float*)malloc(sizeof(float));
        if (!pi) return NULL;
        pi[0] = 1.0f;
        return pi;
    }

    float *pi  = (float*)malloc(n * sizeof(float));
    float *pi2 = (float*)malloc(n * sizeof(float));
    if (!pi || !pi2) { free(pi); free(pi2); return NULL; }

    /* init uniforme */
    for (int j = 0; j < n; ++j) pi[j] = 1.0f / (float)n;

    for (int it = 0; it < max_iter; ++it) {
        /* pi2 = pi * S */
        for (int j = 0; j < n; ++j) pi2[j] = 0.0f;
        for (int k = 0; k < n; ++k) {
            float pik = pi[k];
            if (pik == 0.0f) continue;
            for (int j = 0; j < n; ++j) {
                pi2[j] += pik * S->data[k][j];
            }
        }

        /* normalisation */
        float sum = 0.0f;
        for (int j = 0; j < n; ++j) { if (pi2[j] < 0.0f) pi2[j] = 0.0f; sum += pi2[j]; }
        if (sum > 0.0f) for (int j = 0; j < n; ++j) pi2[j] /= sum;

        /* test convergence L1 */
        float diff = 0.0f;
        for (int j = 0; j < n; ++j) {
            float d = pi2[j] - pi[j];
            if (d < 0) d = -d;
            diff += d;
        }

        /* copie pi2 -> pi */
        for (int j = 0; j < n; ++j) pi[j] = pi2[j];

        if (diff < eps) break;
    }

    free(pi2);
    return pi;   /* à free par l’appelant */
}

/**
 * @brief Frees all memory associated with a matrix.
 *
 * @param M The matrix to destroy.
 */
void DestroyMatrix(p_matrix M) {
    if (M == NULL) {
        return;
    }
    for (int i = 0; i < M->size; i++) {
        free(M->data[i]);
    }
    free(M->data);
    free(M);
}


/**
 * @brief Calculates the greatest common divisor (GCD) of a list of integers.
 * This function is provided by the project subject and requires no changes.
 */
int gcd(int *vals, int nbvals) {
    if (nbvals == 0) return 0;
    int result = vals[0];
    for (int i = 1; i < nbvals; i++) {
        int a = result;
        int b = vals[i];
        while (b != 0) {
            int temp = b;
            b = a % b;
            a = temp;
        }
        result = a;
    }
    return result;
}

/**
 * @brief Calculates the period of a class, adapted to be compatible with your project.
 * @param sub_matrix A POINTER to the sub-matrix (p_matrix).
 * @return The period of the class (1 if aperiodic, >1 if periodic). Returns 0 on error.
 */
int getPeriod(p_matrix sub_matrix) //we put a pointer since we did that in the code
{
    //first we get the size of the matrix in parameter + handle it if the size is 0.
    int n = sub_matrix->size; //we use ->since its now a pointer
    if (n == 0) return 0;

    //allocate a dynamic array so that it can store all the cycle
    int *periods = (int *)malloc(n * sizeof(int));
    if (!periods) return 0; //safety check
    //initialize a counter
    int period_count = 0;

    //initialize a power matrix of size n
    p_matrix power_matrix = CreateEmptyMatrix(n);
    //copy the matrix sub_matrix inside the matrix power matrix so its at the power M^1
    CopyMatrix(power_matrix, sub_matrix);

    //for the main loop we will go from 1 to n calculating M^1, M^2, M^3 each iterations.
    for (int cpt = 1; cpt <= n; cpt++)
    {
        //main loop. In it we search in the main diagnoal if there is a loop for M[i][i] if there is it mean that there is a path in which we can go form i->..->i in cpt times.
        //and then we store it in the period array,  we search for the gcd of the paths (period, period_count) to find the number that fit alll of them.
        int diag_nonzero = 0;
        for (int i = 0; i < n; i++)
        {
            if (power_matrix->data[i][i] > 0.0f) //cahnge for ->data because this is a pointer.
            {
                diag_nonzero = 1;
                break;
            }
        }

        if (diag_nonzero) {
            periods[period_count] = cpt;
            period_count++;
        }

        //we change the power at the power a + 1
        //so we reset the matrix and then we remplace it with the values of M^2
        p_matrix next_power = MultiplyMatrices(power_matrix, sub_matrix);
        DestroyMatrix(power_matrix);
        power_matrix = next_power;
    }


    //we search for the gcd of the period
    int period = gcd(periods, period_count);

    //free not wirte in the algo but needed to understand
    free(periods);
    DestroyMatrix(power_matrix);

    //return the period
    return period;
}
