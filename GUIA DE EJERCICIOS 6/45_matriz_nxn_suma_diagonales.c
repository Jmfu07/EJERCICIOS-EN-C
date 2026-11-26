/*
 * Ejercicio 45 (Matrices): Matriz cuadrada N x N y suma de diagonales
 * -------------------------------------------------------------------
 * QUE HACE:
 *   Genera una matriz cuadrada de N x N con numeros al azar del 0 al 9,
 *   la muestra en pantalla y calcula la sumatoria de la diagonal principal
 *   y de la diagonal inversa (secundaria).
 *
 * COMO FUNCIONA:
 *   - Valida N en el rango [1, 15] con _getch().
 *   - Diagonal principal: elementos donde i == j (m[i][i]).
 *   - Diagonal inversa  : elementos donde j == N - 1 - i (m[i][N - 1 - i]).
 *
 * TIPOS DE DATOS UTILIZADOS:
 *   - int: Dimension N, matriz[15][15], indices y acumuladores de diagonales.
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
    int sumaPrincipal = 0, sumaInversa = 0;

    srand((unsigned int)time(NULL));

    printf("==================================================\n");
    printf("   EJERCICIO 45: MATRIZ N x N SUMA DE DIAGONALES\n");
    printf("==================================================\n\n");

    n = leerEnteroRango("Ingrese la dimension N de la matriz cuadrada", 1, MAX_N);

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            m[i][j] = rand() % 10;
        }
    }

    printf("\nMatriz generada (%d x %d):\n\n", n, n);
    for (i = 0; i < n; i++) {
        printf("   |");
        for (j = 0; j < n; j++) {
            printf(" %2d ", m[i][j]);
        }
        printf("|\n");
        sumaPrincipal += m[i][i];
        sumaInversa += m[i][n - 1 - i];
    }

    printf("\n-> Elementos Diagonal Principal: ");
    for (i = 0; i < n; i++) {
        printf("%d%s", m[i][i], (i < n - 1) ? " + " : "");
    }
    printf(" = %d\n", sumaPrincipal);

    printf("-> Elementos Diagonal Inversa  : ");
    for (i = 0; i < n; i++) {
        printf("%d%s", m[i][n - 1 - i], (i < n - 1) ? " + " : "");
    }
    printf(" = %d\n", sumaInversa);

    printf("\nPresione cualquier tecla para finalizar...");
    _getch();
    return 0;
}
