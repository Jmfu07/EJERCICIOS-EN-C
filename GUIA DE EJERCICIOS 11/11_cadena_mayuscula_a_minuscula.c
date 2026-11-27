/*
 * Guia: Estructuras Secuenciales - Ejercicio 11
 * Enunciado: Realizar un programa en C para leer una cadena de caracteres en
 * mayuscula y convertirla en minuscula. Mostrar resultados con sus respectivos mensajes.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void leer_cadena_mayuscula(const char *prompt, char *destino, size_t tam) {
    while (1) {
        size_t i, len;
        int tiene_letra = 0;
        int tiene_minuscula = 0;

        printf("%s", prompt);
        if (!fgets(destino, (int)tam, stdin)) {
            printf("\nError de lectura.\n");
            exit(1);
        }
        len = strlen(destino);
        while (len > 0 && (destino[len - 1] == '\n' || destino[len - 1] == '\r')) {
            destino[--len] = '\0';
        }
        for (i = 0; i < len; i++) {
            unsigned char c = (unsigned char)destino[i];
            if (isupper(c)) {
                tiene_letra = 1;
            } else if (islower(c)) {
                tiene_minuscula = 1;
            }
        }
        if (!tiene_letra) {
            printf("Debe ingresar una cadena que contenga letras en MAYUSCULA.\n");
            continue;
        }
        if (tiene_minuscula) {
            printf("La cadena contiene letras en minuscula. Ingresela toda en MAYUSCULA.\n");
            continue;
        }
        return;
    }
}

int main(void) {
    char original[128];
    char convertida[128];
    size_t i;

    printf("=== EJERCICIO 11: CONVERTIR CADENA EN MAYUSCULA A MINUSCULA ===\n\n");
    leer_cadena_mayuscula("Ingrese una cadena en MAYUSCULAS: ", original, sizeof(original));

    for (i = 0; original[i] != '\0'; i++) {
        convertida[i] = (char)tolower((unsigned char)original[i]);
    }
    convertida[i] = '\0';

    printf("\nResultados:\n");
    printf("Cadena original en mayuscula   : %s\n", original);
    printf("Cadena convertida en minuscula : %s\n\n", convertida);

    system("pause");
    return 0;
}
