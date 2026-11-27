/*
 * Ejercicio 12:
 * Escribir en Pseudocodigo y codificar en C un programa que muestre los
 * numeros impares entre 0 y 100 y que imprima cuantos impares hay.
 */

#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int i, contador = 0;

    printf("=== EJERCICIO 12: NUMEROS IMPARES ENTRE 0 Y 100 ===\n\n");

    for (i = 1; i <= 100; i += 2) {
        printf("%3d ", i);
        contador++;
        if (contador % 10 == 0) {
            printf("\n");
        }
    }

    printf("\nCantidad total de numeros impares entre 0 y 100: %d\n\n", contador);

    system("pause");
    return 0;
}
