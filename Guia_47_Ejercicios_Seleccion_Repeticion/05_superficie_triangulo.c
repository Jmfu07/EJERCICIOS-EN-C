/*
 * Ejercicio 5:
 * Disenar un programa en C que calcule la superficie de un triangulo a partir
 * del ingreso de su base y altura y muestre el resultado.
 */

#include <stdio.h>
#include <stdlib.h>

static void limpiar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

static double leer_double_positivo(const char *mensaje) {
    double valor;
    char extra;
    while (1) {
        printf("%s", mensaje);
        if (scanf("%lf%c", &valor, &extra) == 2 && (extra == '\n' || extra == '\r')) {
            if (valor > 0.0) {
                return valor;
            }
            printf("El valor debe ser mayor que cero.\n");
        } else {
            printf("Entrada invalida. Por favor ingrese un numero valido.\n");
            limpiar_buffer();
        }
    }
}

int main(void) {
    double base, altura, superficie;

    printf("=== EJERCICIO 5: SUPERFICIE DE UN TRIANGULO ===\n\n");

    base = leer_double_positivo("Ingrese la base del triangulo (> 0): ");
    altura = leer_double_positivo("Ingrese la altura del triangulo (> 0): ");
    superficie = (base * altura) / 2.0;

    printf("\nLa superficie del triangulo (base %.2f, altura %.2f) es: %.2f\n\n",
           base, altura, superficie);

    system("pause");
    return 0;
}
