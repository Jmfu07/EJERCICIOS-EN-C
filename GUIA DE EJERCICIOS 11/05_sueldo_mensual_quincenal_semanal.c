/*
 * Guia: Estructuras Secuenciales - Ejercicio 5
 * Enunciado: Disenar un programa en C que ingrese por teclado nombre, edad,
 * monto de asignacion y monto de deducciones mensuales. Calcule el sueldo neto
 * mensual, quincenal y semanal, mostrar resultados con sus respectivos mensajes.
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
            printf("El texto no puede estar vacio.\n");
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

double leer_real_no_negativo(const char *prompt) {
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
            printf("Entrada invalida. Ingrese un numero real valido.\n");
            continue;
        }
        if (valor < 0.0) {
            printf("El monto no puede ser negativo.\n");
            continue;
        }
        return valor;
    }
}

int main(void) {
    char nombre[100];
    int edad;
    double asignacion, deduccion;
    double sueldo_mensual, sueldo_quincenal, sueldo_semanal;

    printf("=== EJERCICIO 5: SUELDO NETO MENSUAL, QUINCENAL Y SEMANAL ===\n\n");
    leer_cadena_no_vacia("Ingrese el nombre del empleado: ", nombre, sizeof(nombre));
    edad = leer_entero_rango("Ingrese la edad del empleado: ", 14, 120);
    asignacion = leer_real_no_negativo("Ingrese el monto de asignaciones mensuales: ");
    deduccion = leer_real_no_negativo("Ingrese el monto de deducciones mensuales: ");

    sueldo_mensual = asignacion - deduccion;
    sueldo_quincenal = sueldo_mensual / 2.0;
    sueldo_semanal = sueldo_mensual / 4.0;

    printf("\n=== RESUMEN DE PAGO ===\n");
    printf("Empleado              : %s\n", nombre);
    printf("Edad                  : %d anos\n", edad);
    printf("Asignacion mensual    : %.2f\n", asignacion);
    printf("Deduccion mensual     : %.2f\n", deduccion);
    printf("Sueldo neto mensual   : %.2f\n", sueldo_mensual);
    printf("Sueldo neto quincenal : %.2f\n", sueldo_quincenal);
    printf("Sueldo neto semanal   : %.2f\n\n", sueldo_semanal);

    system("pause");
    return 0;
}
