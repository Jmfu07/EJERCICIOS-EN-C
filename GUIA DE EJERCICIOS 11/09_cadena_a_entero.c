/*
 * Guia: Estructuras Secuenciales - Ejercicio 9
 * Enunciado: Realizar un programa para leer una cadena de caracteres y
 * convertirlo en un entero. Mostrar resultados con sus respectivos mensajes.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>

void leer_cadena_entero(const char *prompt, char *cadena, size_t tam, int *resultado) {
    while (1) {
        size_t len;
        char *fin;
        long valor;

        printf("%s", prompt);
        if (!fgets(cadena, (int)tam, stdin)) {
            printf("\nError de lectura.\n");
            exit(1);
        }
        len = strlen(cadena);
        while (len > 0 && (cadena[len - 1] == '\n' || cadena[len - 1] == '\r')) {
            cadena[--len] = '\0';
        }
        if (len == 0) {
            printf("La cadena no puede estar vacia.\n");
            continue;
        }
        errno = 0;
        valor = strtol(cadena, &fin, 10);
        while (*fin == ' ' || *fin == '\t') {
            fin++;
        }
        if (fin == cadena || *fin != '\0') {
            printf("La cadena debe representar un numero entero valido.\n");
            continue;
        }
        if (errno == ERANGE || valor < INT_MIN || valor > INT_MAX) {
            printf("El numero esta fuera del rango de un entero estandar (int).\n");
            continue;
        }
        *resultado = atoi(cadena);
        return;
    }
}

int main(void) {
    char cadena[100];
    int numero;

    printf("=== EJERCICIO 9: CONVERTIR CADENA DE CARACTERES A ENTERO ===\n\n");
    leer_cadena_entero("Ingrese una cadena numerica entera: ", cadena, sizeof(cadena), &numero);

    printf("\nResultados:\n");
    printf("Cadena de caracteres leida : \"%s\"\n", cadena);
    printf("Valor convertido a entero  : %d\n\n", numero);

    system("pause");
    return 0;
}
