#ifndef MATRIX_H
#define MATRIX_H
#include "../adjacency_list/adjacency_list.h"
#include "../tarjan/tarjan.h"

/**
 * @brief Structure representing a square matrix of floating point numbers.
 *
 * Used primarily for storing transition probabilities between vertices.
 */
typedef struct s_matrix {
    int size;
    float **data;
} t_matrix, *p_matrix;

/**
 * @brief Allocates and initializes an empty square matrix (filled with 0.0).
 *
 * @param n The dimension of the matrix (n x n).
 * @return p_matrix Pointer to the allocated matrix.
 */
p_matrix CreateEmptyMatrix(int n);

/**
 * @brief Converts a graph adjacency list into a transition probability matrix.
 *
 * @param graph The source adjacency list.
 * @return p_matrix The resulting transition matrix.
 */
p_matrix CreateMatFromAdjList(t_adjacency_list graph);

/**
 * @brief Copies the data from a source matrix to a destination matrix.
 *
 * @param mat The destination matrix (must be already allocated).
 * @param matsrc The source matrix to copy from.
 */
void CopyMatrix(p_matrix mat, p_matrix matsrc);

/**
 * @brief Multiplies two matrices (A * B).
 *
 * @param A The first matrix.
 * @param B The second matrix.
 * @return p_matrix A new matrix containing the result of the multiplication.
 */
p_matrix MultiplyMatrices(p_matrix A, p_matrix B);

/**
 * @brief Computes the difference (usually sum of absolute differences) between two matrices.
 *
 * @param M The first matrix.
 * @param N The second matrix.
 * @return float The scalar difference representing the distance between M and N.
 */
float DiffMatrix(p_matrix M, p_matrix N);

/**
 * @brief Prints the matrix to the standard output.
 *
 * @param M The matrix to display.
 */
void printMatrix(p_matrix M);

/**
 * @brief Extracts the submatrix corresponding to a specific component (Class) from the partition.
 *
 * @param M The global transition matrix.
 * @param part The partition containing the components.
 * @param compo_index The index of the component to extract.
 * @return p_matrix The submatrix restricted to the vertices of the specified component.
 */
p_matrix SubMatrixByComponent(p_matrix M, t_partition part, int compo_index);

//need to add this to be sure that there is no memory leaks
/**
 * @brief Frees the memory allocated for a matrix.
 *
 * @param M The matrix to destroy.
 */
void DestroyMatrix(p_matrix M);

/**
 * @brief Calculates the Greatest Common Divisor of an array of integers.
 *
 * @param vals Array of integer values.
 * @param nbvals Number of values in the array.
 * @return int The GCD of the values.
 */
int gcd(int *vals, int nbvals);

/**
 * @brief Calculates the period of a submatrix (component).
 *
 * @param sub_matrix The matrix representing the component.
 * @return int The period of the component (1 = aperiodic).
 */
int getPeriod(p_matrix sub_matrix);


/**
 * @brief Computes the stationary probability vector for a given submatrix.
 *
 * @param S The submatrix (stochastic matrix).
 * @param max_iter The maximum number of iterations for the power method.
 * @param eps The convergence threshold (epsilon).
 * @return float* A dynamically allocated array containing the stationary probabilities.
 */
float *StationaryVectorFromSubmatrix(p_matrix S, int max_iter, float eps);
#endif //MATRIX_H