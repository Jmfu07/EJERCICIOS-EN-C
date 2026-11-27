/*
 * Guia: Estructuras Secuenciales - Ejercicio 8
 * Enunciado: Realizar un programa en C para leer una letra mayuscula y
 * convertirla en minuscula. Mostrar resultados con sus respectivos mensajes.
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

char leer_letra_mayuscula(const char *prompt) {
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
        c = buffer[i];
        if (c == '\0' || c == '\n' || c == '\r') {
            printf("Debe ingresar una letra mayuscula (A-Z).\n");
            continue;
        }
        i++;
        while (buffer[i] == ' ' || buffer[i] == '\t') {
            i++;
        }
        if (buffer[i] != '\n' && buffer[i] != '\r' && buffer[i] != '\0') {
            printf("Ingrese unicamente un caracter.\n");
            continue;
        }
        if (c < 'A' || c > 'Z') {
            printf("El caracter ingresado no es una letra mayuscula (A-Z).\n");
            continue;
        }
        return c;
    }
}

int main(void) {
    char mayuscula, minuscula;

    printf("=== EJERCICIO 8: CONVERTIR LETRA MAYUSCULA A MINUSCULA ===\n\n");
    mayuscula = leer_letra_mayuscula("Ingrese una letra mayuscula (A-Z): ");
    minuscula = (char)tolower((unsigned char)mayuscula);

    printf("\nResultados:\n");
    printf("Letra mayuscula ingresada : %c\n", mayuscula);
    printf("Letra convertida a minuscula : %c\n\n", minuscula);

    system("pause");
    return 0;
}
