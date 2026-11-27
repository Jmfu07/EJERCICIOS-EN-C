/*
 * Guia: Ejercicios de Lenguaje C (Progresion I) - Ejercicio 4
 * Enunciado: Escribe un programa que calcule el area de un circulo de radio R
 * y la longitud de su circunferencia. Solicitar el valor de R por teclado,
 * mostrando en la pantalla los mensajes necesarios (S = pi * R^2 ; L = 2 * pi * R).
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
            printf("El radio R debe ser mayor que cero.\n");
            continue;
        }
        return valor;
    }
}

int main(void) {
    double R, S, L;

    printf("=== PROGRESION I - EJERCICIO 4: AREA Y LONGITUD DE UN CIRCULO ===\n\n");
    R = leer_real_positivo("Ingrese el valor del radio R: ");

    S = PI * R * R;
    L = 2.0 * PI * R;

    printf("\nResultados para R = %.2f:\n", R);
    printf("Area del circulo (S = pi * R^2)           = %.4f\n", S);
    printf("Longitud de circunferencia (L = 2 * pi * R) = %.4f\n\n", L);

    system("pause");
    return 0;
}
