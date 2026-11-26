/*
 * Ejercicio 1:
 * Ingresar 5 numeros y calcular su media.
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
    double suma = 0.0;
    int i;

    printf("=== EJERCICIO 1: MEDIA DE 5 NUMEROS ===\n\n");

    for (i = 1; i <= 5; i++) {
        char prompt[64];
        snprintf(prompt, sizeof(prompt), "Ingrese el numero %d: ", i);
        suma += leer_double(prompt);
    }

    printf("\nLa suma de los 5 numeros es: %.2f\n", suma);
    printf("La media (promedio) de los 5 numeros es: %.2f\n\n", suma / 5.0);

    system("pause");
    return 0;
}
