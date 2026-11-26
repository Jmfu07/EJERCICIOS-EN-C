/*
 * Guia: Ejercicios de Lenguaje C (Progresion I) - Ejercicio 8
 * Enunciado: Escribe un programa que lea del teclado un codigo ASCII (entero
 * comprendido entre 0 y 255) y presente en pantalla el caracter correspondiente.
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int leer_codigo_ascii(const char *prompt) {
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
        if (valor < 0 || valor > 255) {
            printf("El codigo ASCII debe estar comprendido entre 0 y 255.\n");
            continue;
        }
        return (int)valor;
    }
}

int main(void) {
    int codigo;

    printf("=== PROGRESION I - EJERCICIO 8: CODIGO ASCII A CARACTER ===\n\n");
    codigo = leer_codigo_ascii("Ingrese un codigo ASCII (0 a 255): ");

    printf("\nResultados:\n");
    printf("Codigo ASCII ingresado   : %d\n", codigo);
    if (codigo < 32 || codigo == 127) {
        printf("Caracter correspondiente : (Caracter de control no imprimible, codigo %d)\n\n", codigo);
    } else {
        printf("Caracter correspondiente : '%c'\n\n", (unsigned char)codigo);
    }

    system("pause");
    return 0;
}
