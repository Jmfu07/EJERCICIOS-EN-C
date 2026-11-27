/*
 * Guia: Ejercicios de Lenguaje C (Progresion I) - Ejercicio 9
 * Enunciado: Escribe un programa que lea del teclado un caracter cualquiera y
 * presente en pantalla su codigo ASCII en decimal, octal y hexadecimal.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned char leer_caracter(const char *prompt) {
    char buffer[128];

    while (1) {
        size_t len;

        printf("%s", prompt);
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            printf("\nError de lectura.\n");
            exit(1);
        }
        len = strlen(buffer);
        while (len > 0 && (buffer[len - 1] == '\n' || buffer[len - 1] == '\r')) {
            buffer[--len] = '\0';
        }
        if (len != 1) {
            printf("Por favor, ingrese exactamente un (1) caracter.\n");
            continue;
        }
        return (unsigned char)buffer[0];
    }
}

int main(void) {
    unsigned char c;

    printf("=== PROGRESION I - EJERCICIO 9: CARACTER A DECIMAL, OCTAL Y HEXADECIMAL ===\n\n");
    c = leer_caracter("Ingrese un caracter cualquiera: ");

    printf("\nResultados para el caracter '%c':\n", c);
    printf("Codigo ASCII en Decimal     : %u\n", (unsigned int)c);
    printf("Codigo ASCII en Octal       : %o\n", (unsigned int)c);
    printf("Codigo ASCII en Hexadecimal : %X\n\n", (unsigned int)c);

    system("pause");
    return 0;
}
