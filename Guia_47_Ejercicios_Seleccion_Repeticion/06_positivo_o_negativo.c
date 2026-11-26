/*
 * Ejercicio 6:
 * Hacer un programa en C que nos permita introducir un numero por teclado y
 * nos informe si es positivo o negativo.
 */

#include <stdio.h>
#include <stdlib.h>

static void limpiar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

static double leer_double(const char *mensaje) {
    double valor;
    char extra;
    while (1) {
        printf("%s", mensaje);
        if (scanf("%lf%c", &valor, &extra) == 2 && (extra == '\n' || extra == '\r')) {
            return valor;
        }
        printf("Entrada invalida. Por favor ingrese un numero valido.\n");
        limpiar_buffer();
    }
}

int main(void) {
    double numero;

    printf("=== EJERCICIO 6: NUMERO POSITIVO O NEGATIVO ===\n\n");

    numero = leer_double("Ingrese un numero: ");

    if (numero > 0.0) {
        printf("\nEl numero %.2f es POSITIVO.\n\n", numero);
    } else if (numero < 0.0) {
        printf("\nEl numero %.2f es NEGATIVO.\n\n", numero);
    } else {
        printf("\nEl numero ingresado es CERO (neutro).\n\n");
    }

    system("pause");
    return 0;
}
