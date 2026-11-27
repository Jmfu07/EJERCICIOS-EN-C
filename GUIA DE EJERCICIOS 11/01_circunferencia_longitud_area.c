/*
 * Guia: Estructuras Secuenciales - Ejercicio 1
 * Enunciado: Disenar un programa en C que calcule la longitud y el area de la
 * circunferencia de radio dado, mostrando los resultados en pantalla con sus
 * mensajes respectivos (longitud = y area = ).
 */

#include <stdio.h>
#include <stdlib.h>

#define PI 3.14159265358979323846

double leer_real_positivo(const char *prompt) {
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
        if (valor <= 0.0) {
            printf("El valor debe ser mayor que cero.\n");
            continue;
        }
        return valor;
    }
}

int main(void) {
    double radio, longitud, area;

    printf("=== EJERCICIO 1: LONGITUD Y AREA DE UNA CIRCUNFERENCIA ===\n\n");
    radio = leer_real_positivo("Ingrese el radio de la circunferencia: ");

    longitud = 2.0 * PI * radio;
    area = PI * radio * radio;

    printf("\nResultados:\n");
    printf("Radio ingresado : %.2f\n", radio);
    printf("longitud = %.4f\n", longitud);
    printf("area = %.4f\n\n", area);

    system("pause");
    return 0;
}
