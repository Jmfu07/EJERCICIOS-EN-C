/*
 * Guia: Estructuras Secuenciales - Ejercicio 6
 * Enunciado: Disenar un programa en C que ingrese por teclado los siguientes
 * datos de un alumno: nombre, edad, nota1, nota2, nota3. Se pide: calcular
 * promedio del estudiante e imprimirlo con su nombre.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void leer_cadena_no_vacia(const char *prompt, char *destino, size_t tam) {
    while (1) {
        size_t i = 0, len;
        int tiene_texto = 0;

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
            if (!isspace((unsigned char)destino[i])) {
                tiene_texto = 1;
                break;
            }
        }
        if (!tiene_texto) {
            printf("El nombre no puede estar vacio.\n");
            continue;
        }
        return;
    }
}

int leer_entero_rango(const char *prompt, int min, int max) {
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
        if (valor < min || valor > max) {
            printf("El valor debe estar entre %d y %d.\n", min, max);
            continue;
        }
        return (int)valor;
    }
}

double leer_nota(const char *prompt) {
    char buffer[128];
    double valor;
    char *fin;

    while (1) {
        printf("%s", prompt);
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            printf("\nError de lectura.\n");
            exit(1);
        }
        valor = strtod(buffer, &fin);
        while (*fin == ' ' || *fin == '\t') {
            fin++;
        }
        if (fin == buffer || (*fin != '\n' && *fin != '\r' && *fin != '\0')) {
            printf("Entrada invalida. Ingrese una nota valida.\n");
            continue;
        }
        if (valor < 0.0 || valor > 20.0) {
            printf("La nota debe estar entre 0 y 20.\n");
            continue;
        }
        return valor;
    }
}

int main(void) {
    char nombre[100];
    int edad;
    double nota1, nota2, nota3, promedio;

    printf("=== EJERCICIO 6: PROMEDIO DE UN ALUMNO (3 NOTAS) ===\n\n");
    leer_cadena_no_vacia("Ingrese el nombre del alumno: ", nombre, sizeof(nombre));
    edad = leer_entero_rango("Ingrese la edad del alumno: ", 5, 120);
    nota1 = leer_nota("Ingrese la nota 1 (0 a 20): ");
    nota2 = leer_nota("Ingrese la nota 2 (0 a 20): ");
    nota3 = leer_nota("Ingrese la nota 3 (0 a 20): ");

    promedio = (nota1 + nota2 + nota3) / 3.0;

    printf("\nResultados:\n");
    printf("Nombre del estudiante : %s\n", nombre);
    printf("Edad                  : %d anos\n", edad);
    printf("Promedio de notas     : %.2f\n\n", promedio);

    system("pause");
    return 0;
}
