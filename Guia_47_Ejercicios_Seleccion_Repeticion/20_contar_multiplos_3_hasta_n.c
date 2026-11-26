/*
 * Ejercicio 20:
 * Hacer un programa en C que nos permita contar los multiplos de 3 desde
 * la unidad hasta un numero que introducimos por teclado.
 */

#include <stdio.h>
#include <stdlib.h>

static void limpiar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

static int leer_natural(const char *mensaje) {
    int valor;
    char extra;
    while (1) {
        printf("%s", mensaje);
        if (scanf("%d%c", &valor, &extra) == 2 && (extra == '\n' || extra == '\r')) {
            if (valor >= 1) {
                return valor;
            }
            printf("El numero debe ser mayor o igual a 1.\n");
        } else {
            printf("Entrada invalida. Ingrese un numero entero valido.\n");
            limpiar_buffer();
        }
    }
}

int main(void) {
    int n, i, contador = 0;

    printf("=== EJERCICIO 20: CONTAR MULTIPLOS DE 3 DESDE 1 HASTA N ===\n\n");

    n = leer_natural("Ingrese un numero entero (>= 1): ");

    printf("\nMultiplos de 3 entre 1 y %d:\n", n);
    for (i = 1; i <= n; i++) {
        if (i % 3 == 0) {
            printf("%4d ", i);
            contador++;
            if (contador % 10 == 0) {
                printf("\n");
            }
        }
    }
    if (contador == 0) {
        printf("(Ninguno)");
    }

    printf("\n\nCantidad total de multiplos de 3 entre 1 y %d: %d\n\n", n, contador);

    system("pause");
    return 0;
}
