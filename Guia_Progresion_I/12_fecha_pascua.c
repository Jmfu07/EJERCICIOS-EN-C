/*
 * Guia: Ejercicios de Lenguaje C (Progresion I) - Ejercicio 12
 * Enunciado: La fecha de Pascua corresponde al primer Domingo despues de la
 * primera Luna llena que sigue al equinoccio de Primavera, y se calcula con las
 * siguientes expresiones (Algoritmo de Gauss):
 * A = resto de (ano / 19)
 * B = resto de (ano / 4)
 * C = resto de (ano / 7)
 * D = resto de (19 * A + 24) / 30
 * E = resto de (2 * B + 4 * C + 6 * D + 5) / 7
 * N = 22 + D + E
 * (Si N <= 31 corresponde al mes de Marzo; si N > 31, N - 31 corresponde a Abril).
 */

#include <stdio.h>
#include <stdlib.h>

int leer_anio(const char *prompt) {
    char buffer[128];
    long valor;
    char *fin;

    while (1) {
        printf("%s", prompt);
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            printf("\nError de lectura.\n");
            exit(1);
        }
        valor = strtol(buffer, &fin, 10);
        while (*fin == ' ' || *fin == '\t') {
            fin++;
        }
        if (fin == buffer || (*fin != '\n' && *fin != '\r' && *fin != '\0')) {
            printf("Entrada invalida. Ingrese un ano valido.\n");
            continue;
        }
        if (valor < 1583 || valor > 9999) {
            printf("Ingrese un ano del calendario Gregoriano entre 1583 y 9999.\n");
            continue;
        }
        return (int)valor;
    }
}

int main(void) {
    int anio, A, B, C, D, E, N;

    printf("=== PROGRESION I - EJERCICIO 12: CALCULO DE LA FECHA DE PASCUA ===\n\n");
    anio = leer_anio("Ingrese el ano (ej. 1900 - 2099): ");

    A = anio % 19;
    B = anio % 4;
    C = anio % 7;
    D = (19 * A + 24) % 30;
    E = (2 * B + 4 * C + 6 * D + 5) % 7;
    N = 22 + D + E;

    printf("\nValores intermedios de Gauss:\n");
    printf("A = %d, B = %d, C = %d, D = %d, E = %d, N = %d\n\n", A, B, C, D, E, N);

    if (N <= 31) {
        printf("En el ano %d, el Domingo de Pascua es el %d de Marzo.\n\n", anio, N);
    } else {
        printf("En el ano %d, el Domingo de Pascua es el %d de Abril.\n\n", anio, N - 31);
    }

    system("pause");
    return 0;
}
