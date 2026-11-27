/*
 * Ejercicio 57 (Matrices): Operaciones entre matriz A (4x4) y vector columna X
 * -------------------------------------------------------------------
 * QUE HACE:
 *   Tomando la matriz A (4x4) del ejercicio 56 y el vector columna
 *   X = [1, -2, 6, 3]^T, calcula y muestra:
 *     a. A * X
 *     b. 3 * A^2 * X
 *     c. (A + A^T) * (2X)
 *     d. (X + 2X)^T * A   (y tambien A * (X + 2X) en forma columna)
 *
 * COMO FUNCIONA:
 *   - A es de 4x4 y X es de 4x1.
 *   - Para a, b y c el resultado es un vector columna de 4x1.
 *   - Para d, como X es 4x1, (X + 2X)^T = (3X)^T es 1x4, que multiplicado
 *     por A (4x4) produce un vector fila de 1x4; ademas se muestra
 *     A * (X + 2X) en 4x1 por si se interpreta como producto a izquierda.
 *
 * TIPOS DE DATOS UTILIZADOS:
 *   - int: Matriz A[4][4], matrices auxiliares y vectores de 4 elementos.
 */

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

void multiplicarMatrizVector(int m[4][4], int v[4], int r[4]) {
    int i, j;
    for (i = 0; i < 4; i++) {
        r[i] = 0;
        for (j = 0; j < 4; j++) {
            r[i] += m[i][j] * v[j];
        }
    }
}

void mostrarVectorColumna(const char *titulo, int v[4]) {
    int i;
    printf("\n%s:\n", titulo);
    for (i = 0; i < 4; i++) {
        printf("   | %6d |\n", v[i]);
    }
}

int main(void) {
    int A[4][4] = {
        { 3,  5, -10,  8},
        { 0, -9,   8,  5},
        { 7,  4,  14, 11},
        { 1,  0,   3, -2}
    };
    int X[4] = {1, -2, 6, 3};

    int resA[4];
    int A2[4][4], A2X[4], resB[4];
    int A_mas_AT[4][4], dosX[4], resC[4];
    int tresX[4], resD_fila[4], resD_col[4];
    int i, j, k;

    printf("============================================================\n");
    printf("  EJERCICIO 57: OPERACIONES MATRIZ A (4x4) Y VECTOR X (4x1)\n");
    printf("============================================================\n\n");

    printf("Matriz A (4x4) y Vector columna X:\n");
    for (i = 0; i < 4; i++) {
        printf("   |");
        for (j = 0; j < 4; j++) {
            printf(" %4d ", A[i][j]);
        }
        printf("|     X[%d] = | %3d |\n", i, X[i]);
    }

    /* a) A * X */
    multiplicarMatrizVector(A, X, resA);
    mostrarVectorColumna("a) Resultado de A * X", resA);

    /* b) 3 * A^2 * X */
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            A2[i][j] = 0;
            for (k = 0; k < 4; k++) {
                A2[i][j] += A[i][k] * A[k][j];
            }
        }
    }
    multiplicarMatrizVector(A2, X, A2X);
    for (i = 0; i < 4; i++) {
        resB[i] = 3 * A2X[i];
    }
    mostrarVectorColumna("b) Resultado de 3 * A^2 * X", resB);

    /* c) (A + A^T) * 2X */
    for (i = 0; i < 4; i++) {
        dosX[i] = 2 * X[i];
        for (j = 0; j < 4; j++) {
            A_mas_AT[i][j] = A[i][j] + A[j][i];
        }
    }
    multiplicarMatrizVector(A_mas_AT, dosX, resC);
    mostrarVectorColumna("c) Resultado de (A + A^T) * 2X", resC);

    /* d) (X + 2X)^T * A  y  A * (X + 2X) */
    for (i = 0; i < 4; i++) {
        tresX[i] = X[i] + 2 * X[i];
    }
    for (j = 0; j < 4; j++) {
        resD_fila[j] = 0;
        for (i = 0; i < 4; i++) {
            resD_fila[j] += tresX[i] * A[i][j];
        }
    }
    multiplicarMatrizVector(A, tresX, resD_col);

    printf("\nd) Resultado de (X + 2X)^T * A (vector fila 1x4):\n   [");
    for (j = 0; j < 4; j++) {
        printf(" %6d ", resD_fila[j]);
    }
    printf("]\n");

    mostrarVectorColumna("   Resultado equivalente como A * (X + 2X) (vector columna 4x1)", resD_col);

    printf("\nPresione cualquier tecla para finalizar...");
    _getch();
    return 0;
}
