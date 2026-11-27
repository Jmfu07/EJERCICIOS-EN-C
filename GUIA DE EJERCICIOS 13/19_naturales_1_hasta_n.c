/*
 * Ejercicio 19:
 * Hacer un programa en C que imprima todos los numeros naturales que
 * hay desde la unidad hasta un numero que introducimos por teclado.
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
            printf("El numero debe ser un natural mayor o igual a 1.\n");
        } else {
            printf("Entrada invalida. Ingrese un numero entero valido.\n");
            limpiar_buffer();
        }
    }
}

int main(void) {
    int n, i;

    printf("=== EJERCICIO 19: NUMEROS NATURALES DESDE 1 HASTA N ===\n\n");

    n = leer_natural("Ingrese un numero natural (>= 1): ");

    printf("\nNumeros naturales desde 1 hasta %d:\n", n);
    for (i = 1; i <= n; i++) {
        printf("%4d ", i);
        if (i % 10 == 0) {
            printf("\n");
        }
    }

    printf("\n\n");
    system("pause");
    return 0;
}
