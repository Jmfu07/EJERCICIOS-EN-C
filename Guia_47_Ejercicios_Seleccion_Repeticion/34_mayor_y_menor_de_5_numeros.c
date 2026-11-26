/*
 * Ejercicio 34:
 * Hacer un programa en C que imprima el mayor y el menor de una serie de
 * 5 numeros que vamos introduciendo por teclado.
 */

#include <stdio.h>
#include <stdlib.h>

static void limpiar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

static double leer_double(int indice) {
    double valor;
    char extra;
    while (1) {
        printf("Ingrese el numero %d de 5: ", indice);
        if (scanf("%lf%c", &valor, &extra) == 2 && (extra == '\n' || extra == '\r')) {
            return valor;
        }
        printf("Entrada invalida. Ingrese un numero valido.\n");
        limpiar_buffer();
    }
}

int main(void) {
    double mayor = 0.0, menor = 0.0;
    int i;

    printf("=== EJERCICIO 34: MAYOR Y MENOR DE 5 NUMEROS ===\n\n");

    for (i = 1; i <= 5; i++) {
        double num = leer_double(i);
        if (i == 1) {
            mayor = num;
            menor = num;
        } else {
            if (num > mayor) mayor = num;
            if (num < menor) menor = num;
        }
    }

    printf("\nResultados:\n");
    printf("- El MAYOR de los 5 numeros es: %.2f\n", mayor);
    printf("- El MENOR de los 5 numeros es: %.2f\n\n", menor);

    system("pause");
    return 0;
}
