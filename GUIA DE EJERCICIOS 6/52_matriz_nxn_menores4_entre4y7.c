/*
 * Ejercicio 52 (Matrices): Cantidad de numeros < 4 y entre 4 y 7 en matriz N x N
 * -------------------------------------------------------------------
 * QUE HACE:
 *   Genera una matriz cuadrada de N x N con numeros al azar del 0 al 9
 *   y determina la cantidad de numeros menores a 4 (0, 1, 2, 3) y la
 *   cantidad de numeros entre 4 y 7 (4, 5, 6, 7).
 *
 * COMO FUNCIONA:
 *   - Valida N en [1, 15].
 *   - Recorre la matriz contando cuantos elementos cumplen m[i][j] < 4
 *     y cuantos cumplen m[i][j] >= 4 && m[i][j] <= 7.
 *
 * TIPOS DE DATOS UTILIZADOS:
 *   - int: Dimension N, matriz m[15][15], indices y contadores.
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
    int menores4 = 0, entre4y7 = 0;

    srand((unsigned int)time(NULL));

    printf("==========================================================\n");
    printf("  EJERCICIO 52: CANTIDAD MENORES A 4 Y ENTRE 4 Y 7 (NxN)\n");
    printf("==========================================================\n\n");

    n = leerEnteroRango("Ingrese la dimension N de la matriz cuadrada", 1, MAX_N);

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            m[i][j] = rand() % 10;
            if (m[i][j] < 4) {
                menores4++;
            } else if (m[i][j] >= 4 && m[i][j] <= 7) {
                entre4y7++;
            }
        }
    }

    printf("\nMatriz generada (%d x %d):\n\n", n, n);
    for (i = 0; i < n; i++) {
        printf("   |");
        for (j = 0; j < n; j++) {
            printf(" %2d ", m[i][j]);
        }
        printf("|\n");
    }

    printf("\n-> Cantidad de numeros menores a 4 (0..3)   : %d\n", menores4);
    printf("-> Cantidad de numeros entre 4 y 7 (4..7)   : %d\n", entre4y7);
    printf("-> Cantidad de numeros mayores a 7 (8..9)   : %d\n", (n * n) - menores4 - entre4y7);

    printf("\nPresione cualquier tecla para finalizar...");
    _getch();
    return 0;
}
