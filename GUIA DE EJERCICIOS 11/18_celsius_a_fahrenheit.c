/*
 * Guia: Estructuras Secuenciales - Ejercicio 18
 * Enunciado: Escribir un programa en C que me permita leer un numero decimal que
 * representa una cantidad de grados Celsius y convierta dicho valor a la cantidad
 * equivalente en grados Fahrenheit. La salida del programa puede ser de la forma
 * siguiente: 100 grados Celsius son 212 grados Fahrenheit.
 */

#include <stdio.h>
#include <stdlib.h>

double leer_real(const char *prompt) {
    char buffer[128];
    double valor;
    char *fin;

    while (1) {
        printf("%s", prompt);
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            printf("\nError de lectura.\n");
            exit(1);
        }
        valor = strtod(buffer, &fin);
        while (*fin == ' ' || *fin == '\t') {
            fin++;
        }
        if (fin == buffer || (*fin != '\n' && *fin != '\r' && *fin != '\0')) {
            printf("Entrada invalida. Ingrese un numero decimal valido.\n");
            continue;
        }
        return valor;
    }
}

int main(void) {
    double celsius, fahrenheit;

    printf("=== EJERCICIO 18: CONVERSION DE GRADOS CELSIUS A FAHRENHEIT ===\n\n");
    celsius = leer_real("Ingrese la temperatura en grados Celsius: ");

    fahrenheit = (9.0 / 5.0) * celsius + 32.0;

    printf("\nResultado:\n");
    printf("%.2f grados Celsius son %.2f grados Fahrenheit\n\n", celsius, fahrenheit);

    system("pause");
    return 0;
}
