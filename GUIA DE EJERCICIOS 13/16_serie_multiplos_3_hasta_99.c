/*
 * Ejercicio 16:
 * Disenar un programa en C que imprima y sume la serie de numeros
 * 3, 6, 9, 12, ..., 99.
 */

#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int i, suma = 0, contador = 0;

    printf("=== EJERCICIO 16: SERIE 3, 6, 9, ..., 99 Y SU SUMA ===\n\n");

    for (i = 3; i <= 99; i += 3) {
        printf("%2d%s", i, (i < 99) ? ", " : "\n");
        suma += i;
        contador++;
        if (contador % 11 == 0 && i < 99) {
            printf("\n");
        }
    }

    printf("\nLa suma total de la serie 3, 6, 9, ..., 99 es: %d\n\n", suma);

    system("pause");
    return 0;
}
