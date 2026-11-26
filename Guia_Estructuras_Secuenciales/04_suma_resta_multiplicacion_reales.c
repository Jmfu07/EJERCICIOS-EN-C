/*
 * Guia: Estructuras Secuenciales - Ejercicio 4
 * Enunciado: Disenar un programa en C que calcule la suma, la resta y la
 * multiplicacion, ingresando por teclado dos numeros reales, imprima resultados
 * con sus mensajes respectivos.
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
            printf("Entrada invalida. Ingrese un numero real valido.\n");
            continue;
        }
        return valor;
    }
}

int main(void) {
    double num1, num2, suma, resta, multiplicacion;

    printf("=== EJERCICIO 4: SUMA, RESTA Y MULTIPLICACION DE DOS REALES ===\n\n");
    num1 = leer_real("Ingrese el primer numero real: ");
    num2 = leer_real("Ingrese el segundo numero real: ");

    suma = num1 + num2;
    resta = num1 - num2;
    multiplicacion = num1 * num2;

    printf("\nResultados:\n");
    printf("Suma           (%.2f + %.2f) = %.2f\n", num1, num2, suma);
    printf("Resta          (%.2f - %.2f) = %.2f\n", num1, num2, resta);
    printf("Multiplicacion (%.2f * %.2f) = %.2f\n\n", num1, num2, multiplicacion);

    system("pause");
    return 0;
}
