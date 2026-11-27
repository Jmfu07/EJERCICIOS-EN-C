/* GUIA DE EJERCICIOS 5 - EJERCICIO 5:
 * Que muestre los primeros 100 numeros de izquierda a derecha usando un array de dos dimensiones,
 * y la ultima fila (fila 11) mostrara la suma de sus respectivas columnas.
 */
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int m[11][10] = {{0}};
    int f, c, num = 1;

    printf("=== PRIMEROS 100 NUMEROS Y ULTIMA FILA CON SUMA DE COLUMNAS ===\n\n");
    for (f = 0; f < 10; f++) {
        for (c = 0; c < 10; c++) {
            m[f][c] = num++;
            m[10][c] += m[f][c];
            printf("%5d", m[f][c]);
        }
        printf("\n");
    }

    printf("--------------------------------------------------\n");
    for (c = 0; c < 10; c++)
        printf("%5d", m[10][c]);
    printf("  <- Suma de cada columna\n\n");

    system("pause");
    return 0;
}
