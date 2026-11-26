/*
 * Guia: Estructuras Secuenciales - Ejercicio 7
 * Enunciado: Realizar un programa en C para leer una letra minuscula y
 * convertirla en mayuscula. Mostrar resultados con sus respectivos mensajes.
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

char leer_letra_minuscula(const char *prompt) {
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
            printf("Debe ingresar una letra minuscula (a-z).\n");
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
        if (c < 'a' || c > 'z') {
            printf("El caracter ingresado no es una letra minuscula (a-z).\n");
            continue;
        }
        return c;
    }
}

int main(void) {
    char minuscula, mayuscula;

    printf("=== EJERCICIO 7: CONVERTIR LETRA MINUSCULA A MAYUSCULA ===\n\n");
    minuscula = leer_letra_minuscula("Ingrese una letra minuscula (a-z): ");
    mayuscula = (char)toupper((unsigned char)minuscula);

    printf("\nResultados:\n");
    printf("Letra minuscula ingresada : %c\n", minuscula);
    printf("Letra convertida a mayuscula : %c\n\n", mayuscula);

    system("pause");
    return 0;
}
