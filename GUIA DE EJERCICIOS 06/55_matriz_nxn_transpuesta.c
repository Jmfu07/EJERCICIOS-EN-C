/*
 * Ejercicio 55 (Matrices): Matriz cuadrada N x N y su transpuesta
 * -------------------------------------------------------------------
 * QUE HACE:
 *   Genera una matriz cuadrada de N x N con numeros al azar del 0 al 9,
 *   la muestra en pantalla, calcula su matriz transpuesta y la muestra.
 *
 * COMO FUNCIONA:
 *   - Valida N en [1, 15] con _getch().
 *   - Construye la matriz transpuesta asignando t[j][i] = m[i][j].
 *
 * TIPOS DE DATOS UTILIZADOS:
 *   - int: Dimension N, matriz original m[15][15], transpuesta t[15][15], indices.
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
    int m[MAX_N][MAX_N], t[MAX_N][MAX_N];
    int n, i, j;

    srand((unsigned int)time(NULL));

    printf("=======================================================\n");
    printf("   EJERCICIO 55: MATRIZ CUADRADA N x N Y TRANSPUESTA\n");
    printf("=======================================================\n\n");

    n = leerEnteroRango("Ingrese la dimension N de la matriz cuadrada", 1, MAX_N);

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            m[i][j] = rand() % 10;
            t[j][i] = m[i][j];
        }
    }

    printf("\nMatriz Original (%d x %d):\n\n", n, n);
    for (i = 0; i < n; i++) {
        printf("   |");
        for (j = 0; j < n; j++) {
            printf(" %2d ", m[i][j]);
        }
        printf("|\n");
    }

    printf("\nMatriz Transpuesta (%d x %d):\n\n", n, n);
    for (i = 0; i < n; i++) {
        printf("   |");
        for (j = 0; j < n; j++) {
            printf(" %2d ", t[i][j]);
        }
        printf("|\n");
    }

    printf("\nPresione cualquier tecla para finalizar...");
    _getch();
    return 0;
}
