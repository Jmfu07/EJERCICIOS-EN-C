/*
 * Ejercicio 48 (Matrices): Sumatoria de los bordes de una matriz N x N
 * -------------------------------------------------------------------
 * QUE HACE:
 *   Genera una matriz cuadrada de N x N con numeros al azar del 0 al 9,
 *   la muestra en pantalla y calcula la sumatoria de todos los elementos
 *   de sus bordes sin repetir la suma de sus 4 vertices.
 *
 * COMO FUNCIONA:
 *   - Un elemento (i, j) pertenece al borde si:
 *       i == 0  ||  i == N - 1  ||  j == 0  ||  j == N - 1
 *   - Al evaluar esa condicion una sola vez por celda (i, j), los
 *     vertices se suman exactamente una vez.
 *
 * TIPOS DE DATOS UTILIZADOS:
 *   - int: Dimension N, matriz m[15][15], indices y sumaBordes.
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
    int n, i, j, sumaBordes = 0;

    srand((unsigned int)time(NULL));

    printf("=======================================================\n");
    printf("  EJERCICIO 48: SUMATORIA DE BORDES EN MATRIZ N x N\n");
    printf("=======================================================\n\n");

    n = leerEnteroRango("Ingrese la dimension N de la matriz cuadrada", 2, MAX_N);

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            m[i][j] = rand() % 10;
            if (i == 0 || i == n - 1 || j == 0 || j == n - 1) {
                sumaBordes += m[i][j];
            }
        }
    }

    printf("\nMatriz completa (%d x %d):\n\n", n, n);
    for (i = 0; i < n; i++) {
        printf("   |");
        for (j = 0; j < n; j++) {
            printf(" %2d ", m[i][j]);
        }
        printf("|\n");
    }

    printf("\nElementos del borde considerados (sin repetir vertices):\n\n");
    for (i = 0; i < n; i++) {
        printf("   |");
        for (j = 0; j < n; j++) {
            if (i == 0 || i == n - 1 || j == 0 || j == n - 1) {
                printf(" %2d ", m[i][j]);
            } else {
                printf("  . ");
            }
        }
        printf("|\n");
    }

    printf("\n-> Sumatoria de los bordes (sin repetir vertices): %d\n", sumaBordes);

    printf("\nPresione cualquier tecla para finalizar...");
    _getch();
    return 0;
}
