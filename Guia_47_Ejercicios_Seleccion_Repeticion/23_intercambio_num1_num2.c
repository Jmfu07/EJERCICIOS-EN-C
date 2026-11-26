/*
 * Ejercicio 23:
 * Disenar un programa en C necesario para que habiendose leido el valor
 * de 2 variables NUM1 y NUM2 se intercambien los valores de las variables, es
 * decir que el valor que tenia NUM1 ahora lo contenga NUM2 y viceversa.
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
    double NUM1, NUM2, aux;

    printf("=== EJERCICIO 23: INTERCAMBIO DE VARIABLES NUM1 Y NUM2 ===\n\n");

    NUM1 = leer_double("Ingrese el valor de NUM1: ");
    NUM2 = leer_double("Ingrese el valor de NUM2: ");

    printf("\nAntes del intercambio:\n");
    printf("NUM1 = %.2f, NUM2 = %.2f\n", NUM1, NUM2);

    aux = NUM1;
    NUM1 = NUM2;
    NUM2 = aux;

    printf("\nDespues del intercambio:\n");
    printf("NUM1 = %.2f, NUM2 = %.2f\n\n", NUM1, NUM2);

    system("pause");
    return 0;
}
