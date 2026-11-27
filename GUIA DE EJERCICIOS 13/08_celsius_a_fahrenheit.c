/*
 * Ejercicio 8:
 * Escribir un programa en C que me permita leer un numero decimal que
 * representa una cantidad de grados Celsius y convierta dicho valor a la cantidad
 * equivalente en grados Fahrenheit. La salida del programa puede ser de la
 * siguiente forma: 100 grados Celsius son 212 grados Fahrenheit
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
        printf("Entrada invalida. Por favor ingrese un numero decimal valido.\n");
        limpiar_buffer();
    }
}

int main(void) {
    double celsius, fahrenheit;

    printf("=== EJERCICIO 8: CONVERSION CELSIUS A FAHRENHEIT ===\n\n");

    celsius = leer_double("Ingrese la temperatura en grados Celsius: ");
    fahrenheit = (celsius * 9.0 / 5.0) + 32.0;

    printf("\n%g grados Celsius son %g grados Fahrenheit\n", celsius, fahrenheit);
    printf("(Con 2 decimales: %.2f grados Celsius son %.2f grados Fahrenheit)\n\n",
           celsius, fahrenheit);

    system("pause");
    return 0;
}
