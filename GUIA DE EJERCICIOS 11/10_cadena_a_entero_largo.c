/*
 * Guia: Estructuras Secuenciales - Ejercicio 10
 * Enunciado: Realizar un programa en C para leer una cadena de caracteres y
 * convertirla en un entero largo. Mostrar resultados con sus respectivos mensajes.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

void leer_cadena_entero_largo(const char *prompt, char *cadena, size_t tam, long *resultado) {
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
            printf("La cadena debe representar un numero entero largo valido.\n");
            continue;
        }
        if (errno == ERANGE) {
            printf("El numero esta fuera del rango de un entero largo (long).\n");
            continue;
        }
        *resultado = valor;
        return;
    }
}

int main(void) {
    char cadena[100];
    long numero_largo;

    printf("=== EJERCICIO 10: CONVERTIR CADENA DE CARACTERES A ENTERO LARGO ===\n\n");
    leer_cadena_entero_largo("Ingrese una cadena numerica para convertir a entero largo: ",
                             cadena, sizeof(cadena), &numero_largo);

    printf("\nResultados:\n");
    printf("Cadena de caracteres leida      : \"%s\"\n", cadena);
    printf("Valor convertido a entero largo : %ld\n\n", numero_largo);

    system("pause");
    return 0;
}
