/* GUIA DE EJERCICIOS 5 - EJERCICIO 4:
 * Que muestre los primeros 100 numeros de izquierda a derecha usando
 * un array de dos dimensiones (10x10).
 */
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int m[10][10];
    int f, c, num = 1;

    printf("=== PRIMEROS 100 NUMEROS EN MATRIZ 10x10 (IZQUIERDA A DERECHA) ===\n\n");
    for (f = 0; f < 10; f++) {
        for (c = 0; c < 10; c++) {
            m[f][c] = num++;
            printf("%4d", m[f][c]);
        }
        printf("\n");
    }
    printf("\n");
    system("pause");
    return 0;
}
