/*
 * Guia: Ejercicios de Lenguaje C (Progresion I) - Ejercicio 13
 * Enunciado: Ingresar una opcion numerica y realizar las siguientes actividades:
 * a.- (Opcion 1) Ingrese por teclado un valor numerico de A y calcule el valor
 *     de K. K = sqrt(A).
 * b.- (Opcion 2) Ingrese por teclado tres valores A, B y C. Intercambie el
 *     valor de B en A y C en B.
 * c.- (Opcion 3) Ingrese por teclado el Nombre, Edad, Sexo y Promedio de un
 *     Alumno. Imprima el Nombre y el Sexo si el alumno esta aprobado en caso
 *     contrario imprimir un mensaje que diga Reprobado.
 * Validar todas las opciones e incluso en caso incorrecto. Imprimir los
 * resultados de cada actividad.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

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
            printf("Este campo no puede estar vacio.\n");
            continue;
        }
        return;
    }
}

int leer_entero(const char *prompt) {
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
        return (int)valor;
    }
}

double leer_real(const char *prompt) {
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
            printf("Entrada invalida. Ingrese un numero valido.\n");
            continue;
        }
        return valor;
    }
}

char leer_sexo(const char *prompt) {
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
        if (c != 'M' && c != 'F') {
            printf("Ingrese 'M' para Masculino o 'F' para Femenino.\n");
            continue;
        }
        i++;
        while (buffer[i] == ' ' || buffer[i] == '\t') {
            i++;
        }
        if (buffer[i] != '\n' && buffer[i] != '\r' && buffer[i] != '\0') {
            printf("Ingrese unicamente una letra (M o F).\n");
            continue;
        }
        return c;
    }
}

int main(void) {
    int opcion;

    printf("=== PROGRESION I - EJERCICIO 13: MENU DE ACTIVIDADES ===\n\n");
    printf("1.- Calcular K = raiz cuadrada de A\n");
    printf("2.- Intercambiar valores entre A, B y C (B en A y C en B)\n");
    printf("3.- Evaluar datos de un Alumno (Aprobado / Reprobado)\n\n");

    opcion = leer_entero("Seleccione una opcion numerica (1, 2 o 3): ");

    switch (opcion) {
        case 1: {
            double A, K;
            printf("\n--- Actividad 1: K = sqrt(A) ---\n");
            while (1) {
                A = leer_real("Ingrese un valor numerico A (>= 0): ");
                if (A < 0.0) {
                    printf("Error: No existe raiz cuadrada real de un numero negativo.\n");
                    continue;
                }
                break;
            }
            K = sqrt(A);
            printf("Para A = %.4f, el valor de K = sqrt(A) es: %.4f\n\n", A, K);
            break;
        }
        case 2: {
            double A, B, C, auxA;
            printf("\n--- Actividad 2: Intercambio entre A, B y C ---\n");
            A = leer_real("Ingrese el valor de A: ");
            B = leer_real("Ingrese el valor de B: ");
            C = leer_real("Ingrese el valor de C: ");

            printf("\nValores originales : A = %.2f, B = %.2f, C = %.2f\n", A, B, C);
            auxA = A;
            A = B;
            B = C;
            C = auxA;
            printf("Valores finales    : A = %.2f, B = %.2f, C = %.2f\n\n", A, B, C);
            break;
        }
        case 3: {
            char nombre[100];
            int edad;
            char sexo;
            double promedio;

            printf("\n--- Actividad 3: Evaluacion de Alumno ---\n");
            leer_cadena_no_vacia("Ingrese el Nombre del alumno: ", nombre, sizeof(nombre));
            while (1) {
                edad = leer_entero("Ingrese la Edad del alumno (5 a 120): ");
                if (edad < 5 || edad > 120) {
                    printf("Edad fuera de rango (5 a 120).\n");
                    continue;
                }
                break;
            }
            sexo = leer_sexo("Ingrese el Sexo del alumno (M/F): ");
            while (1) {
                promedio = leer_real("Ingrese el Promedio del alumno (0 a 20): ");
                if (promedio < 0.0 || promedio > 20.0) {
                    printf("El promedio debe estar entre 0 y 20.\n");
                    continue;
                }
                break;
            }

            printf("\nResultado de la evaluacion:\n");
            if (promedio >= 10.0) {
                printf("Estado  : APROBADO (Promedio: %.2f, Edad: %d)\n", promedio, edad);
                printf("Nombre  : %s\n", nombre);
                printf("Sexo    : %s\n\n", (sexo == 'M') ? "Masculino (M)" : "Femenino (F)");
            } else {
                printf("Alumno %s: REPROBADO (Promedio: %.2f)\n\n", nombre, promedio);
            }
            break;
        }
        default:
            printf("\nOpcion incorrecta: %d. Debe elegir una opcion entre 1 y 3.\n\n", opcion);
            break;
    }

    system("pause");
    return 0;
}
