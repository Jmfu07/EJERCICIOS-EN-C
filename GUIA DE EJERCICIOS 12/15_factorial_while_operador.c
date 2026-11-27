/*
 * Guia: Ejercicios de Lenguaje C (Progresion I) - Ejercicio 15
 * Enunciado: Leer un numero entero y calcular el factorial. Validar todas las
 * opciones posibles. Utilizar el ciclo while con respuesta al operador.
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int leer_entero_factorial(const char *prompt) {
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
            printf("Entrada invalida. Ingrese un numero entero.\n");
            continue;
        }
        if (valor < 0) {
            printf("No existe el factorial de un numero negativo. Ingrese un entero >= 0.\n");
            continue;
        }
        if (valor > 20) {
            printf("Por favor ingrese un numero entre 0 y 20 para evitar desbordamiento de 64 bits.\n");
            continue;
        }
        return (int)valor;
    }
}

char leer_respuesta(const char *prompt) {
    char buffer[128];

    while (1) {
        size_t i = 0;
        char c;

        printf("%s", prompt);
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            printf("\nError de lectura.\n");
            exit(1);
        }
        while (buffer[i] == ' ' || buffer[i] == '\t') {
            i++;
        }
        c = (char)toupper((unsigned char)buffer[i]);
        if (c != 'S' && c != 'N') {
            printf("Respuesta invalida. Ingrese 'S' para Si o 'N' para No.\n");
            continue;
        }
        i++;
        while (buffer[i] == ' ' || buffer[i] == '\t') {
            i++;
        }
        if (buffer[i] != '\n' && buffer[i] != '\r' && buffer[i] != '\0') {
            printf("Ingrese unicamente una letra ('S' o 'N').\n");
            continue;
        }
        return c;
    }
}

int main(void) {
    char respuesta = 'S';

    printf("=== PROGRESION I - EJERCICIO 15: CALCULO DE FACTORIAL (CICLO WHILE) ===\n\n");

    while (respuesta == 'S') {
        int n, i;
        unsigned long long factorial = 1ULL;

        n = leer_entero_factorial("Ingrese un numero entero (0 a 20): ");
        i = 1;
        while (i <= n) {
            factorial *= (unsigned long long)i;
            i++;
        }

        printf("-> El factorial de %d (%d!) es: %llu\n\n", n, n, factorial);
        respuesta = leer_respuesta("Desea calcular el factorial de otro numero? (S/N): ");
        printf("\n");
    }

    system("pause");
    return 0;
}
