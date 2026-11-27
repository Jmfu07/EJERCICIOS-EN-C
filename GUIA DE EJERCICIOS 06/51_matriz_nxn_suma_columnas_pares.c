/*
 * Ejercicio 51 (Matrices): Sumatoria de numeros en columnas pares de matriz N x N
 * -------------------------------------------------------------------
 * QUE HACE:
 *   Genera una matriz cuadrada de N x N con numeros al azar del 0 al 9
 *   y calcula la sumatoria de los numeros ubicados en las columnas pares.
 *
 * COMO FUNCIONA:
 *   - Calcula tanto la sumatoria de columnas pares numeradas 2, 4, 6...
 *     ((j + 1) % 2 == 0) como por indice C par (0, 2, 4..., j % 2 == 0).
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
    int sumaColParesBase1 = 0;
    int sumaColParesIndiceC = 0;

    srand((unsigned int)time(NULL));

    printf("==========================================================\n");
    printf(" EJERCICIO 51: SUMATORIA DE COLUMNAS PARES (MATRIZ N x N)\n");
    printf("==========================================================\n\n");

    n = leerEnteroRango("Ingrese la dimension N de la matriz cuadrada", 1, MAX_N);

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            m[i][j] = rand() % 10;
            if ((j + 1) % 2 == 0) {
                sumaColParesBase1 += m[i][j];
            }
            if (j % 2 == 0) {
                sumaColParesIndiceC += m[i][j];
            }
        }
    }

    printf("\nMatriz generada (%d x %d):\n\n", n, n);
    printf("     ");
    for (j = 0; j < n; j++) {
        printf(" C%-2d", j + 1);
    }
    printf("\n");

    for (i = 0; i < n; i++) {
        printf("F%-2d |", i + 1);
        for (j = 0; j < n; j++) {
            printf(" %2d ", m[i][j]);
        }
        printf("|\n");
    }

    printf("\n-> Sumatoria de columnas pares (Columnas 2, 4, 6...): %d\n", sumaColParesBase1);
    printf("-> Sumatoria por indice C par  (Indices 0, 2, 4...) : %d\n", sumaColParesIndiceC);

    printf("\nPresione cualquier tecla para finalizar...");
    _getch();
    return 0;
}
