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
    int anio, a, b, c, d, e, f, g, h, i, k, l, m, mes, dia;

    printf("=== PROGRESION I - EJERCICIO 12: CALCULO DE LA FECHA DE PASCUA ===\n\n");
    anio = leer_anio("Ingrese el ano Gregoriano (1583 - 9999): ");

    /* Algoritmo Gregoriano completo (Meeus/Jones/Butcher), valido para todo ano >= 1583 */
    a = anio % 19;
    b = anio / 100;
    c = anio % 100;
    d = b / 4;
    e = b % 4;
    f = (b + 8) / 25;
    g = (b - f + 1) / 3;
    h = (19 * a + b - d - g + 15) % 30;
    i = c / 4;
    k = c % 4;
    l = (32 + 2 * e + 2 * i - h - k) % 7;
    m = (a + 11 * h + 22 * l) / 451;
    mes = (h + l - 7 * m + 114) / 31;
    dia = ((h + l - 7 * m + 114) % 31) + 1;

    printf("\nEn el ano %d, el Domingo de Pascua es el %d de %s.\n\n",
           anio, dia, (mes == 3) ? "Marzo" : "Abril");

    system("pause");
    return 0;
}
