/*
 * Guia: Ejercicios de Lenguaje C (Progresion I) - Ejercicio 2
 * Enunciado: Ingrese por teclado los siguientes datos de un alumno: Nombre,
 * Nota uno, Nota dos y Nota tres. Se pide calcular el promedio de las
 * calificaciones del estudiante e imprimirlo con su nombre.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void leer_cadena_no_vacia(const char *prompt, char *destino, size_t tam) {
    while (1) {
        size_t i, len;
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
            printf("Entrada invalida. Ingrese una calificacion numerica.\n");
            continue;
        }
        if (valor < 0.0 || valor > 20.0) {
            printf("La calificacion debe estar entre 0 y 20.\n");
            continue;
        }
        return valor;
    }
}

int main(void) {
    char nombre[100];
    double nota1, nota2, nota3, promedio;

    printf("=== PROGRESION I - EJERCICIO 2: PROMEDIO DE 3 NOTAS ===\n\n");
    leer_cadena_no_vacia("Ingrese el nombre del alumno: ", nombre, sizeof(nombre));
    nota1 = leer_nota("Ingrese la Nota uno (0 a 20): ");
    nota2 = leer_nota("Ingrese la Nota dos (0 a 20): ");
    nota3 = leer_nota("Ingrese la Nota tres (0 a 20): ");

    promedio = (nota1 + nota2 + nota3) / 3.0;

    printf("\nResultados:\n");
    printf("Alumno   : %s\n", nombre);
    printf("Promedio : %.2f\n\n", promedio);

    system("pause");
    return 0;
}
