/*
 * Ejercicio 56 (Matrices): Operaciones algebraicas con matrices A y B de 4x4
 * -------------------------------------------------------------------
 * QUE HACE:
 *   Dadas las matrices 4x4 A y B del enunciado (con opcion de usar las
 *   del libro o editarlas), efectua y muestra las operaciones:
 *     a. 2A - B
 *     b. (A + B) * A
 *     c. A^2 + B^2
 *     d. (A - B)^2
 *
 * COMO FUNCIONA:
 *   - En el PDF (de "Caminando junto al lenguaje C", p. 130) aparecen
 *     los 16 elementos de la matriz 4x4:
 *       [  3   5 -10   8 ]
 *       [  0  -9   8   5 ]
 *       [  7   4  14  11 ]
 *       [  1   0   3  -2 ]
 *     y la segunda matriz:
 *       [ 10   0  -3  -8 ]
 *       [ 20   1   4   5 ]
 *       [ 17  -4   4   9 ]
 *       [  5   1   9  11 ]
 *   - Implementa funciones modulares para suma, resta, producto por
 *     escalar y multiplicacion matricial 4x4.
 *
 * TIPOS DE DATOS UTILIZADOS:
 *   - int: Matrices 4x4 A, B y matrices de resultados parciales y finales.
 */

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

void mostrarMatriz(const char *titulo, int m[4][4]) {
    int i, j;
    printf("\n%s:\n", titulo);
    for (i = 0; i < 4; i++) {
        printf("   |");
        for (j = 0; j < 4; j++) {
            printf(" %6d ", m[i][j]);
        }
        printf("|\n");
    }
}

void sumarMatrices(int a[4][4], int b[4][4], int r[4][4]) {
    int i, j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            r[i][j] = a[i][j] + b[i][j];
        }
    }
}

void restarMatrices(int a[4][4], int b[4][4], int r[4][4]) {
    int i, j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            r[i][j] = a[i][j] - b[i][j];
        }
    }
}

void multiplicarMatrices(int a[4][4], int b[4][4], int r[4][4]) {
    int i, j, k;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            r[i][j] = 0;
            for (k = 0; k < 4; k++) {
                r[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

int main(void) {
    int A[4][4] = {
        { 3,  5, -10,  8},
        { 0, -9,   8,  5},
        { 7,  4,  14, 11},
        { 1,  0,   3, -2}
    };
    int B[4][4] = {
        {10,  0, -3, -8},
        {20,  1,  4,  5},
        {17, -4,  4,  9},
        { 5,  1,  9, 11}
    };

    int resA[4][4];
    int A_mas_B[4][4], resB[4][4];
    int A2[4][4], B2[4][4], resC[4][4];
    int A_menos_B[4][4], resD[4][4];
    int i, j;

    printf("============================================================\n");
    printf("  EJERCICIO 56: OPERACIONES CON MATRICES A Y B (4x4)\n");
    printf("============================================================\n");

    mostrarMatriz("Matriz A", A);
    mostrarMatriz("Matriz B", B);

    /* a) 2A - B */
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            resA[i][j] = 2 * A[i][j] - B[i][j];
        }
    }
    mostrarMatriz("a) Resultado de 2A - B", resA);

    /* b) (A + B) * A */
    sumarMatrices(A, B, A_mas_B);
    multiplicarMatrices(A_mas_B, A, resB);
    mostrarMatriz("b) Resultado de (A + B) * A", resB);

    /* c) A^2 + B^2 */
    multiplicarMatrices(A, A, A2);
    multiplicarMatrices(B, B, B2);
    sumarMatrices(A2, B2, resC);
    mostrarMatriz("c) Resultado de A^2 + B^2", resC);

    /* d) (A - B)^2 */
    restarMatrices(A, B, A_menos_B);
    multiplicarMatrices(A_menos_B, A_menos_B, resD);
    mostrarMatriz("d) Resultado de (A - B)^2", resD);

    printf("\nPresione cualquier tecla para finalizar...");
    _getch();
    return 0;
}
