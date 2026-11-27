/*
 * Ejercicio 15:
 * Hacer un programa en C que imprima los numeros del 100 al 0 en orden
 * decreciente.
 */

#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int i, contador = 0;

    printf("=== EJERCICIO 15: NUMEROS DEL 100 AL 0 EN ORDEN DECRECIENTE ===\n\n");

    for (i = 100; i >= 0; i--) {
        printf("%3d ", i);
        contador++;
        if (contador % 10 == 0) {
            printf("\n");
        }
    }

    printf("\n\n");
    system("pause");
    return 0;
}
