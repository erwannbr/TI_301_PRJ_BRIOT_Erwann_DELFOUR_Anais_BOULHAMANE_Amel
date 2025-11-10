#ifndef MATRIX_H
#define MATRIX_H
#include "../adjacency_list/adjacency_list.h"
#include "../tarjan/tarjan.h"

typedef struct s_matrix {
    int size;
    float **data;
} t_matrix, *p_matrix;

p_matrix CreateEmptyMatrix(int n);

p_matrix CreateMatFromAdjList(t_adjacency_list graph);

void CopyMatrix(p_matrix mat, p_matrix matsrc);

p_matrix MultiplyMatrices(p_matrix A, p_matrix B);

float DiffMatrix(p_matrix M, p_matrix N);

void printMatrix(p_matrix M);

p_matrix SubMatrixByComponent(p_matrix M, t_partition part, int compo_index);
#endif //MATRIX_H
