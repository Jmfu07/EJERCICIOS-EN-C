/*
 * Guia: Ejercicios de Lenguaje C (Progresion I) - Ejercicio 21
 * Enunciado: Dada la matriz de 4x4:
 *       [ 2  -1   4   0 ]
 *   P = [ 1   2  -1   3 ]
 *       [ 5   2  -1   4 ]
 *       [ 8   9  10   1 ]
 * Se pide:
 * a) Cargar la matriz P y generar la matriz transpuesta Pt.
 * b) Sumar los elementos de la diagonal principal de la matriz transpuesta.
 * c) Imprimir ambas matrices.
 */

#include <stdio.h>
#include <stdlib.h>

#define N 4

int leer_entero(const char *prompt) {
    char buffer[128];
    long valor;
    char *fin;

    while (1) {
        printf("%s", prompt);
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            printf("\nError de lectura.\n");
            exit(1);
        }
        valor = strtol(buffer, &fin, 10);
        while (*fin == ' ' || *fin == '\t') {
            fin++;
        }
        if (fin == buffer || (*fin != '\n' && *fin != '\r' && *fin != '\0')) {
            printf("Entrada invalida. Ingrese un numero entero.\n");
            continue;
        }
        return (int)valor;
    }
}

void imprimir_matriz(const char *titulo, int M[N][N]) {
    int i, j;
    printf("%s:\n", titulo);
    for (i = 0; i < N; i++) {
        printf("  [");
        for (j = 0; j < N; j++) {
            printf("%4d", M[i][j]);
        }
        printf("   ]\n");
    }
    printf("\n");
}

int main(void) {
    int P[N][N] = {
        { 2, -1,  4,  0},
        { 1,  2, -1,  3},
        { 5,  2, -1,  4},
        { 8,  9, 10,  1}
    };
    int Pt[N][N];
    int i, j, modo, suma_diagonal = 0;

    printf("=== PROGRESION I - EJERCICIO 21: MATRIZ 4x4, TRANSPUESTA Y DIAGONAL ===\n\n");
    printf("1.- Usar la matriz P de 4x4 dada en el enunciado de la guia\n");
    printf("2.- Cargar manualmente los 16 elementos de la matriz P por teclado\n\n");

    while (1) {
        modo = leer_entero("Seleccione una opcion (1 o 2): ");
        if (modo == 1 || modo == 2) {
            break;
        }
        printf("Opcion invalida. Ingrese 1 o 2.\n");
    }

    if (modo == 2) {
        printf("\n--- Carga de la matriz P (4x4) ---\n");
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                char prompt[64];
                snprintf(prompt, sizeof(prompt), "Ingrese P[%d][%d]: ", i + 1, j + 1);
                P[i][j] = leer_entero(prompt);
            }
        }
    }

    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            Pt[j][i] = P[i][j];
        }
    }

    for (i = 0; i < N; i++) {
        suma_diagonal += Pt[i][i];
    }

    printf("\n");
    imprimir_matriz("Matriz Original P (4x4)", P);
    imprimir_matriz("Matriz Transpuesta Pt (4x4)", Pt);

    printf("Elementos de la diagonal principal de Pt: ");
    for (i = 0; i < N; i++) {
        printf("%d%s", Pt[i][i], (i < N - 1) ? " + " : " ");
    }
    printf("= %d\n\n", suma_diagonal);

    system("pause");
    return 0;
}
