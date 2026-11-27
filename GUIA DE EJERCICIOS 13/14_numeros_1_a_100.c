/*
 * Ejercicio 14:
 * Hacer un programa en C que imprima los numeros del 1 al 100.
 */

#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int i;

    printf("=== EJERCICIO 14: NUMEROS DEL 1 AL 100 ===\n\n");

    for (i = 1; i <= 100; i++) {
        printf("%3d ", i);
        if (i % 10 == 0) {
            printf("\n");
        }
    }

    printf("\n");
    system("pause");
    return 0;
}
