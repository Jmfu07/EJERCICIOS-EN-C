/*
 * Guia: Ejercicios de Lenguaje C (Progresion I) - Ejercicio 14
 * Enunciado: Ingresar por teclado los siguientes datos de un conjunto de
 * empleados: nombre, edad, sexo, monto de asignacion mensual y monto de
 * deduccion mensuales. Se pide:
 * a) Calcular el sueldo actual del empleado e imprimirlo con su nombre.
 * b) Edad promedio de los hombres.
 * c) Porcentaje de mujeres y hombres.
 * d) Cuantos sueldos netos mensuales son mayores a 450 Bs.F.
 * Utilizar el haga mientras (do-while) con una respuesta del operador.
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
            printf("Entrada invalida. Ingrese un monto valido.\n");
            continue;
        }
        if (valor < 0.0) {
            printf("El monto no puede ser negativo.\n");
            continue;
        }
        return valor;
    }
}

char leer_opcion_char(const char *prompt, char op1, char op2) {
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
        if (c != op1 && c != op2) {
            printf("Entrada invalida. Ingrese '%c' o '%c'.\n", op1, op2);
            continue;
        }
        i++;
        while (buffer[i] == ' ' || buffer[i] == '\t') {
            i++;
        }
        if (buffer[i] != '\n' && buffer[i] != '\r' && buffer[i] != '\0') {
            printf("Ingrese unicamente un caracter ('%c' o '%c').\n", op1, op2);
            continue;
        }
        return c;
    }
}

int main(void) {
    char respuesta;
    int total_empleados = 0;
    int total_hombres = 0;
    int total_mujeres = 0;
    int suma_edad_hombres = 0;
    int sueldos_mayores_450 = 0;

    printf("=== PROGRESION I - EJERCICIO 14: ESTADISTICAS DE EMPLEADOS (DO-WHILE) ===\n");

    do {
        char nombre[100];
        int edad;
        char sexo;
        double asignacion, deduccion, sueldo_neto;

        printf("\n--- Registro del Empleado #%d ---\n", total_empleados + 1);
        leer_cadena_no_vacia("Nombre del empleado: ", nombre, sizeof(nombre));
        edad = leer_entero_rango("Edad del empleado (14 a 120): ", 14, 120);
        sexo = leer_opcion_char("Sexo del empleado (M = Masculino, F = Femenino): ", 'M', 'F');
        asignacion = leer_real_no_negativo("Monto de asignacion mensual (Bs.F): ");
        deduccion = leer_real_no_negativo("Monto de deduccion mensual (Bs.F): ");

        sueldo_neto = asignacion - deduccion;
        total_empleados++;

        if (sexo == 'M') {
            total_hombres++;
            suma_edad_hombres += edad;
        } else {
            total_mujeres++;
        }

        if (sueldo_neto > 450.0) {
            sueldos_mayores_450++;
        }

        printf("\n-> a) Empleado: %s | Sueldo neto actual: %.2f Bs.F.\n\n", nombre, sueldo_neto);

        respuesta = leer_opcion_char("Desea ingresar otro empleado? (S/N): ", 'S', 'N');
    } while (respuesta == 'S');

    printf("\n==================================================\n");
    printf("               RESUMEN ESTADISTICO                \n");
    printf("==================================================\n");
    printf("Total de empleados procesados : %d\n", total_empleados);
    if (total_hombres > 0) {
        printf("b) Edad promedio de hombres   : %.2f anos\n",
               (double)suma_edad_hombres / (double)total_hombres);
    } else {
        printf("b) Edad promedio de hombres   : No se registraron hombres\n");
    }
    printf("c) Porcentaje de hombres      : %.2f%%\n",
           ((double)total_hombres * 100.0) / (double)total_empleados);
    printf("   Porcentaje de mujeres      : %.2f%%\n",
           ((double)total_mujeres * 100.0) / (double)total_empleados);
    printf("d) Sueldos netos > 450 Bs.F.  : %d\n", sueldos_mayores_450);
    printf("==================================================\n\n");

    system("pause");
    return 0;
}
