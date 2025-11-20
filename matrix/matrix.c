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
            printf("%.2f  ",M->data[i][j]);
        }
        printf("]\n");
    }
    printf("\n");
    printf("\n");
}

p_matrix SubMatrixByComponent(p_matrix M, t_partition part, int compo_index) {
    if (M == NULL) return NULL;
    if (compo_index < 0 || compo_index >= part.nb_class) return NULL;

    t_class *classe = part.classes[compo_index];
    int n = classe->nb_vertices;
    if (n <= 0) return NULL;

    p_matrix sub = CreateEmptyMatrix(n);

    for (int i = 0; i < n; i++) {
        int row = classe->vertices[i] - 1;
        for (int j = 0; j < n; j++) {
            int col = classe->vertices[j] - 1;
            sub->data[i][j] = M->data[row][col];
        }
    }

    return sub;
}

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
