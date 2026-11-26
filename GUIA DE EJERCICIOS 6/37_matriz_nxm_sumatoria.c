/*
 * Ejercicio 37 (Matrices): Matriz N x M al azar y sumatoria total
 * -------------------------------------------------------------------
 * QUE HACE:
 *   Genera una matriz de N filas por M columnas con numeros al azar
 *   del 0 al 9, la muestra por pantalla y calcula la sumatoria de
 *   todos sus elementos.
 *
 * COMO FUNCIONA:
 *   - Valida las dimensiones N y M con _getch() dentro de [1, 20].
 *   - Mientras carga cada elemento con rand() % 10, lo acumula en 'suma'.
 *   - Muestra la matriz y el resultado de la sumatoria.
 *
 * TIPOS DE DATOS UTILIZADOS:
 *   - int: Dimensiones N y M, matriz, indices y acumulador 'suma'.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

#define MAX_FILAS 20
#define MAX_COLUMNAS 20

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
    int matriz[MAX_FILAS][MAX_COLUMNAS];
    int n, m, i, j, suma = 0;

    srand((unsigned int)time(NULL));

    printf("==================================================\n");
    printf("   EJERCICIO 37: MATRIZ N x M Y SUMATORIA TOTAL\n");
    printf("==================================================\n\n");

    n = leerEnteroRango("Ingrese cantidad de filas (N)", 1, MAX_FILAS);
    m = leerEnteroRango("Ingrese cantidad de columnas (M)", 1, MAX_COLUMNAS);

    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++) {
            matriz[i][j] = rand() % 10;
            suma += matriz[i][j];
        }
    }

    printf("\nMatriz generada (%d x %d):\n\n", n, m);
    for (i = 0; i < n; i++) {
        printf("   |");
        for (j = 0; j < m; j++) {
            printf(" %2d ", matriz[i][j]);
        }
        printf("|\n");
    }

    printf("\n-> Sumatoria total de los elementos: %d\n", suma);

    printf("\nPresione cualquier tecla para finalizar...");
    _getch();
    return 0;
}
