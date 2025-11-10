//
// Created by anais on 10/11/2025.
//
//
// Created by anais on 20/10/2025.
//


#include "matrix.h"
#include "../cell/cell.h"
#include "../utils/utils.h"
#include <stdio.h>
#include <stdlib.h>


p_matrix CreateEmptyMatrix(int n) {
    p_matrix M = (p_matrix)malloc(sizeof(t_matrix));
    if (!M) {
        fprintf(stderr, "Erreur d'allocation de la matrice.\n");
        exit(EXIT_FAILURE);
    }

    M->size = n;
    M->data = malloc(n * sizeof(float *));
    for (int i = 0; i < n; i++) {
        M->data[i] = calloc(n, sizeof(float));
    }

    return M;
}
p_matrix CreateMatFromAdjList(t_adjacency_list graph) {
    int n = graph.size;
    p_matrix M = CreateEmptyMatrix(n);

    for (int i = 0; i < n; i++) {
        t_cell *neigh = graph.array[i].head;
        while (neigh != NULL) {
            int j = neigh->arrival - 1;
            M->data[i][j] = neigh->probability;
            neigh = neigh->next;
        }
    }

    return M;
}
void CopyMatrix(p_matrix mat, p_matrix matsrc) {
    if (mat->size != matsrc->size) {
        fprintf(stderr, "Erreur : matrices de tailles différentes.\n");
        return;
    }

    for (int i = 0; i < matsrc->size; i++)
        for (int j = 0; j < matsrc->size; j++)
            mat->data[i][j] = matsrc->data[i][j];
}
p_matrix MultiplyMatrices(p_matrix A, p_matrix B) {
    if (A->size != B->size) {
        fprintf(stderr, "Erreur : tailles incompatibles pour la multiplication.\n");
        return NULL;
    }

    int n = A->size;
    p_matrix R = CreateEmptyMatrix(n);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            float sum = 0.0f;
            for (int k = 0; k < n; k++) {
                sum += A->data[i][k] * B->data[k][j];
            }
            R->data[i][j] = sum;
        }
    }

    return R;
}
float DiffMatrix(p_matrix M, p_matrix N) {
    if (M->size != N->size) {
        fprintf(stderr, "Erreur : matrices de tailles différentes pour diffMatrix.\n");
        return -1;
    }

    float diff;
    float somme = 0.0f;
    for (int i = 0; i < M->size; i++)
        for (int j = 0; j < M->size; j++) {
            diff = M->data[i][j] - N->data[i][j];
            if (diff < 0) diff = -diff;
            somme += diff;
        }


    return somme;
}
void printMatrix(p_matrix M) {
    int n = M->size;
    for (int i = 0; i<n ; i++) {
        printf("[");
        for (int j = 0; j<n ; j++) {
            printf("%d  ",M[i][j]);
        }
        printf("]\n");
    }
}