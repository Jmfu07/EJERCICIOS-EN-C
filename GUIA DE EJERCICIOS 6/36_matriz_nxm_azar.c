/*
 * Ejercicio 36 (Matrices): Matriz N x M al azar con constantes max_filas y max_columnas
 * -------------------------------------------------------------------
 * QUE HACE:
 *   Solicita al usuario la cantidad de filas (n) y columnas (m),
 *   declarando la matriz en memoria con constantes MAX_FILAS y MAX_COLUMNAS,
 *   la llena con numeros aleatorios del 0 al 9 y la muestra por pantalla.
 *
 * COMO FUNCIONA:
 *   - Lee n y m validando digito por digito con _getch() y verificando
 *     que esten dentro del rango [1, MAX_FILAS] y [1, MAX_COLUMNAS].
 *   - Genera los elementos aleatorios con rand() % 10 y los imprime.
 *
 * TIPOS DE DATOS UTILIZADOS:
 *   - int: Dimensiones n, m, matriz[MAX_FILAS][MAX_COLUMNAS] e indices.
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
        printf("   [!] Error: el valor debe estar entre %d y %d.\n", min, max);
    }
}

int main(void) {
    int m_azar[MAX_FILAS][MAX_COLUMNAS];
    int n, m, i, j;

    srand((unsigned int)time(NULL));

    printf("==================================================\n");
    printf("      EJERCICIO 36: MATRIZ N x M AL AZAR (0-9)\n");
    printf("==================================================\n\n");

    n = leerEnteroRango("Ingrese cantidad de filas (n)", 1, MAX_FILAS);
    m = leerEnteroRango("Ingrese cantidad de columnas (m)", 1, MAX_COLUMNAS);

    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++) {
            m_azar[i][j] = rand() % 10;
        }
    }

    printf("\nMatriz generada de %d x %d:\n\n", n, m);
    for (i = 0; i < n; i++) {
        printf("   |");
        for (j = 0; j < m; j++) {
            printf(" %2d ", m_azar[i][j]);
        }
        printf("|\n");
    }

    printf("\nPresione cualquier tecla para finalizar...");
    _getch();
    return 0;
}
