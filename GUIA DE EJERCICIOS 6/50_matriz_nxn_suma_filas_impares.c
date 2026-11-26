/*
 * Ejercicio 50 (Matrices): Sumatoria de numeros en filas impares de matriz N x N
 * -------------------------------------------------------------------
 * QUE HACE:
 *   Genera una matriz cuadrada de N x N con numeros al azar del 0 al 9
 *   y calcula la sumatoria de los numeros ubicados en las filas impares.
 *
 * COMO FUNCIONA:
 *   - Muestra tanto el criterio por numero de fila humano (Filas 1, 3, 5...,
 *     es decir, indice C par: i % 2 == 0) como por indice C impar (indices
 *     1, 3, 5...), para que coincida con cualquier convencion del docente.
 *
 * TIPOS DE DATOS UTILIZADOS:
 *   - int: Dimension N, matriz m[15][15], indices y acumuladores de suma.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

#define MAX_N 15

int leerEnteroRango(const char *mensaje, int min, int max) {
    char buf[16];
    int idx, c, val;
    while (1) {
        printf("%s (%d a %d): ", mensaje, min, max);
        idx = 0;
        while (1) {
            c = _getch();
            if (c == 0 || c == 224) {
                _getch();
                continue;
            }
            if (c == '\r' && idx > 0) {
                buf[idx] = '\0';
                printf("\n");
                break;
            } else if (c == '\b' && idx > 0) {
                idx--;
                printf("\b \b");
            } else if (c >= '0' && c <= '9' && idx < 9) {
                buf[idx++] = (char)c;
                printf("%c", c);
            }
        }
        val = atoi(buf);
        if (val >= min && val <= max) {
            return val;
        }
        printf("   [!] Error: ingrese un numero entre %d y %d.\n", min, max);
    }
}

int main(void) {
    int m[MAX_N][MAX_N];
    int n, i, j;
    int sumaFilasImparesBase1 = 0;
    int sumaFilasImparesIndiceC = 0;

    srand((unsigned int)time(NULL));

    printf("==========================================================\n");
    printf("  EJERCICIO 50: SUMATORIA DE FILAS IMPARES (MATRIZ N x N)\n");
    printf("==========================================================\n\n");

    n = leerEnteroRango("Ingrese la dimension N de la matriz cuadrada", 1, MAX_N);

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            m[i][j] = rand() % 10;
            if ((i + 1) % 2 != 0) {
                sumaFilasImparesBase1 += m[i][j];
            }
            if (i % 2 != 0) {
                sumaFilasImparesIndiceC += m[i][j];
            }
        }
    }

    printf("\nMatriz generada (%d x %d):\n\n", n, n);
    for (i = 0; i < n; i++) {
        printf("Fila %2d (ind %2d) |", i + 1, i);
        for (j = 0; j < n; j++) {
            printf(" %2d ", m[i][j]);
        }
        printf("|%s\n", ((i + 1) % 2 != 0) ? " <-- Fila impar (1,3,5...)" : "");
    }

    printf("\n-> Sumatoria de filas impares (Filas 1, 3, 5...): %d\n", sumaFilasImparesBase1);
    printf("-> Sumatoria por indice C impar (Indices 1, 3..): %d\n", sumaFilasImparesIndiceC);

    printf("\nPresione cualquier tecla para finalizar...");
    _getch();
    return 0;
}
