/*
 * Ejercicio 3:
 * De 10 numeros ingresados indicar cuantos son mayores a cero y cuantos
 * son menores a cero.
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
    int mayores = 0;
    int menores = 0;
    int iguales = 0;
    int i;

    printf("=== EJERCICIO 3: 10 NUMEROS MAYORES Y MENORES A CERO ===\n\n");

    for (i = 1; i <= 10; i++) {
        char prompt[64];
        double num;
        snprintf(prompt, sizeof(prompt), "Ingrese el numero %d de 10: ", i);
        num = leer_double(prompt);
        if (num > 0.0) {
            mayores++;
        } else if (num < 0.0) {
            menores++;
        } else {
            iguales++;
        }
    }

    printf("\nResultados:\n");
    printf("- Cantidad de numeros mayores a cero: %d\n", mayores);
    printf("- Cantidad de numeros menores a cero: %d\n", menores);
    if (iguales > 0) {
        printf("- Cantidad de numeros iguales a cero: %d\n", iguales);
    }
    printf("\n");

    system("pause");
    return 0;
}
