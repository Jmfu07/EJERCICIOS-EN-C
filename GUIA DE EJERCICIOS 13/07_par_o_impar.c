/*
 * Ejercicio 7:
 * Hacer un programa en C que nos permita introducir un numero por teclado y
 * nos informe si es par o impar.
 */

#include <stdio.h>
#include <stdlib.h>

static void limpiar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

static int leer_entero(const char *mensaje) {
    int valor;
    char extra;
    while (1) {
        printf("%s", mensaje);
        if (scanf("%d%c", &valor, &extra) == 2 && (extra == '\n' || extra == '\r')) {
            return valor;
        }
        printf("Entrada invalida. Por favor ingrese un numero entero valido.\n");
        limpiar_buffer();
    }
}

int main(void) {
    int numero;

    printf("=== EJERCICIO 7: NUMERO PAR O IMPAR ===\n\n");

    numero = leer_entero("Ingrese un numero entero: ");

    if (numero % 2 == 0) {
        printf("\nEl numero %d es PAR.\n\n", numero);
    } else {
        printf("\nEl numero %d es IMPAR.\n\n", numero);
    }

    system("pause");
    return 0;
}
