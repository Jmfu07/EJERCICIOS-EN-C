/*
 * Ejercicio 13:
 * Hacer un programa en C que imprima los numeros pares entre el 1 y el 100.
 */

#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int i, contador = 0;

    printf("=== EJERCICIO 13: NUMEROS PARES ENTRE 1 Y 100 ===\n\n");

    for (i = 2; i <= 100; i += 2) {
        printf("%3d ", i);
        contador++;
        if (contador % 10 == 0) {
            printf("\n");
        }
    }

    printf("\nTotal de numeros pares impresos: %d\n\n", contador);

    system("pause");
    return 0;
}
