/*
 * Guia: Estructuras Secuenciales - Ejercicio 19
 * Enunciado: Por teclado se ingresa el valor hora de un empleado. Posteriormente
 * se ingresa el nombre del empleado, la antiguedad y la cantidad de horas
 * trabajadas en el mes. Se pide calcular el importe a cobrar teniendo en cuenta
 * que al total que resulta de multiplicar el valor hora por la cantidad de horas
 * trabajadas, hay que sumarle la cantidad de anos trabajados multiplicados por
 * $30, y el total de todas esas operaciones restarle el 13% en concepto de
 * descuentos. Imprimir el recibo correspondiente con el nombre, la antiguedad,
 * el valor hora, el total a cobrar en bruto, el total de descuentos y el valor
 * a cobrar.
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

double leer_real_positivo(const char *prompt) {
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
        if (valor <= 0.0) {
            printf("El valor debe ser mayor que cero.\n");
            continue;
        }
        return valor;
    }
}

int leer_entero_no_negativo(const char *prompt, int max) {
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
        if (valor < 0 || valor > max) {
            printf("El valor debe estar entre 0 y %d.\n", max);
            continue;
        }
        return (int)valor;
    }
}

int main(void) {
    double valor_hora, horas_trabajadas;
    char nombre[100];
    int antiguedad;
    double total_bruto, total_descuentos, neto_cobrar;

    printf("=== EJERCICIO 19: RECIBO DE SUELDO CON ANTIGUEDAD ===\n\n");
    valor_hora = leer_real_positivo("Ingrese el valor por hora ($): ");
    leer_cadena_no_vacia("Ingrese el nombre del empleado: ", nombre, sizeof(nombre));
    antiguedad = leer_entero_no_negativo("Ingrese la antiguedad en anos: ", 80);
    horas_trabajadas = leer_real_positivo("Ingrese la cantidad de horas trabajadas en el mes: ");

    total_bruto = (valor_hora * horas_trabajadas) + (antiguedad * 30.0);
    total_descuentos = total_bruto * 0.13;
    neto_cobrar = total_bruto - total_descuentos;

    printf("\n============================================\n");
    printf("              RECIBO DE SUELDO              \n");
    printf("============================================\n");
    printf("Nombre del empleado     : %s\n", nombre);
    printf("Antiguedad              : %d ano(s)\n", antiguedad);
    printf("Valor por hora          : $%.2f\n", valor_hora);
    printf("Horas trabajadas        : %.2f\n", horas_trabajadas);
    printf("--------------------------------------------\n");
    printf("Total a cobrar en bruto : $%.2f\n", total_bruto);
    printf("Total descuentos (13%%)  : $%.2f\n", total_descuentos);
    printf("Valor neto a cobrar     : $%.2f\n", neto_cobrar);
    printf("============================================\n\n");

    system("pause");
    return 0;
}
