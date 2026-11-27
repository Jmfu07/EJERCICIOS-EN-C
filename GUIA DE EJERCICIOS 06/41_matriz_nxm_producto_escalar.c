/*
 * Ejercicio 41 (Matrices): Producto de una matriz N x M por un escalar
 * -------------------------------------------------------------------
 * QUE HACE:
 *   Genera una matriz de N x M con numeros al azar del 1 al 9, solicita
 *   un escalar por teclado, calcula el producto en una nueva matriz y
 *   muestra ambas matrices por pantalla.
 *
 * COMO FUNCIONA:
 *   - Valida N y M en el rango [1, 15] y el escalar entero (positivo,
 *     negativo o cero) con _getch().
 *   - Rellena la matriz original con 1 + rand() % 9.
 *   - En una segunda matriz 'res', almacena original[i][j] * escalar.
 *
 * TIPOS DE DATOS UTILIZADOS:
 *   - int: Dimensiones N y M, escalar, matriz original y matriz resultante.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

#define MAX_FILAS 15
#define MAX_COLUMNAS 15

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
            if (c == '\r' && idx > 0 && !(idx == 1 && buf[0] == '-')) {
                buf[idx] = '\0';
                printf("\n");
                break;
            } else if (c == '\b' && idx > 0) {
                idx--;
                printf("\b \b");
            } else if (c == '-' && idx == 0 && min < 0) {
                buf[idx++] = '-';
                printf("-");
            } else if (c >= '0' && c <= '9' && idx < 9) {
                buf[idx++] = (char)c;
                printf("%c", c);
            }
        }
        val = atoi(buf);
        if (val >= min && val <= max) {
            return val;
        }
        printf("   [!] Error: ingrese un valor entre %d y %d.\n", min, max);
    }
}

int main(void) {
    int orig[MAX_FILAS][MAX_COLUMNAS];
    int res[MAX_FILAS][MAX_COLUMNAS];
    int n, m, escalar, i, j;

    srand((unsigned int)time(NULL));

    printf("==================================================\n");
    printf("   EJERCICIO 41: PRODUCTO DE MATRIZ POR ESCALAR\n");
    printf("==================================================\n\n");

    n = leerEnteroRango("Ingrese cantidad de filas (N)", 1, MAX_FILAS);
    m = leerEnteroRango("Ingrese cantidad de columnas (M)", 1, MAX_COLUMNAS);
    escalar = leerEnteroRango("Ingrese el valor del escalar", -1000, 1000);

    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++) {
            orig[i][j] = 1 + rand() % 9;
            res[i][j] = orig[i][j] * escalar;
        }
    }

    printf("\nMatriz Original (%d x %d) [valores 1 al 9]:\n\n", n, m);
    for (i = 0; i < n; i++) {
        printf("   |");
        for (j = 0; j < m; j++) {
            printf(" %4d ", orig[i][j]);
        }
        printf("|\n");
    }

    printf("\nMatriz Resultante (Multiplicada por %d):\n\n", escalar);
    for (i = 0; i < n; i++) {
        printf("   |");
        for (j = 0; j < m; j++) {
            printf(" %5d ", res[i][j]);
        }
        printf("|\n");
    }

    printf("\nPresione cualquier tecla para finalizar...");
    _getch();
    return 0;
}
