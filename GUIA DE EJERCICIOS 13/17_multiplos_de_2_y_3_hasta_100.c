/*
 * Ejercicio 17:
 * Escribir en Pseudocodigo y codificar en C un programa que muestre los
 * multiplos de 2 y de 3 y de ambos comprendidos entre 0 y 100.
 */

#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int i, col;

    printf("=== EJERCICIO 17: MULTIPLOS DE 2, DE 3 Y DE AMBOS ENTRE 0 Y 100 ===\n\n");

    printf("Multiplos de 2 (entre 0 y 100):\n");
    col = 0;
    for (i = 0; i <= 100; i++) {
        if (i % 2 == 0) {
            printf("%3d ", i);
            if (++col % 13 == 0) printf("\n");
        }
    }

    printf("\n\nMultiplos de 3 (entre 0 y 100):\n");
    col = 0;
    for (i = 0; i <= 100; i++) {
        if (i % 3 == 0) {
            printf("%3d ", i);
            if (++col % 12 == 0) printf("\n");
        }
    }

    printf("\n\nMultiplos de ambos (de 2 y de 3, es decir de 6) entre 0 y 100:\n");
    col = 0;
    for (i = 0; i <= 100; i++) {
        if (i % 2 == 0 && i % 3 == 0) {
            printf("%3d ", i);
            if (++col % 12 == 0) printf("\n");
        }
    }

    printf("\n\n");
    system("pause");
    return 0;
}
