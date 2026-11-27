/*
 * Guia: Ejercicios de Lenguaje C (Progresion I) - Ejercicio 16
 * Enunciado: Ingrese por teclado los siguientes datos de un conjunto de alumnos:
 * Nombre, edad, nota uno con ponderacion del 45%, nota dos con ponderacion del
 * 30% y nota tres con la ponderacion restante (25%). Se pide:
 * a) Calcula la Nota final del estudiante e imprimirlo con su nombre.
 * b) Cuantos alumnos aprobaron y reprobaron.
 * Utilizar el ciclo while con respuestas del operador.
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
            printf("Respuesta invalida. Ingrese 'S' o 'N'.\n");
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
    int aprobados = 0, reprobados = 0;

    printf("=== PROGRESION I - EJERCICIO 16: NOTAS PONDERADAS (45%%, 30%%, 25%%) ===\n");

    while (respuesta == 'S') {
        char nombre[100];
        int edad;
        double n1, n2, n3, nota_final;

        printf("\n--- Datos del Alumno #%d ---\n", aprobados + reprobados + 1);
        leer_cadena_no_vacia("Nombre del alumno: ", nombre, sizeof(nombre));
        edad = leer_entero_rango("Edad del alumno (5 a 120): ", 5, 120);
        n1 = leer_nota("Nota 1 (45%%, escala 0 a 20): ");
        n2 = leer_nota("Nota 2 (30%%, escala 0 a 20): ");
        n3 = leer_nota("Nota 3 (25%%, escala 0 a 20): ");

        nota_final = (n1 * 0.45) + (n2 * 0.30) + (n3 * 0.25);

        if (nota_final >= 10.0) {
            aprobados++;
        } else {
            reprobados++;
        }

        printf("\n-> a) Estudiante: %s (Edad: %d) | Nota Final: %.2f (%s)\n\n",
               nombre, edad, nota_final, (nota_final >= 10.0) ? "APROBADO" : "REPROBADO");

        respuesta = leer_respuesta("Desea ingresar otro alumno? (S/N): ");
    }

    printf("\n=== RESUMEN FINAL ===\n");
    printf("b) Total de alumnos aprobados  : %d\n", aprobados);
    printf("   Total de alumnos reprobados : %d\n\n", reprobados);

    system("pause");
    return 0;
}
