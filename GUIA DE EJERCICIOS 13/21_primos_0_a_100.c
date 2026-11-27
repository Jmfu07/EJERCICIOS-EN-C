/*
 * Ejercicio 21:
 * Escribir en Pseudocodigo y codificar en C un programa que muestre los
 * numeros primos comprendidos entre 0 y 100.
 */

#include <stdio.h>
#include <stdlib.h>

static int es_primo(int n) {
    int d;
    if (n < 2) return 0;
    for (d = 2; d * d <= n; d++) {
        if (n % d == 0) return 0;
    }
    return 1;
}

int main(void) {
    int i, contador = 0;

    printf("=== EJERCICIO 21: NUMEROS PRIMOS ENTRE 0 Y 100 ===\n\n");

    for (i = 0; i <= 100; i++) {
        if (es_primo(i)) {
            printf("%3d ", i);
            contador++;
            if (contador % 10 == 0) {
                printf("\n");
            }
        }
    }

    printf("\n\nTotal de numeros primos entre 0 y 100: %d\n\n", contador);

    system("pause");
    return 0;
}
