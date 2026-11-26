/*
 * Ejercicio 25:
 * Disenar en Pseudocodigo y codificar en C un algoritmo que permita
 * registrar de los empleados de una fabrica (no se sabe cuantos) su peso y saber
 * cuantos pesan hasta 80 kg. inclusive y cuantos pesan mas de 80 kg.
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

static void limpiar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

static double leer_peso(int num_empleado) {
    double peso;
    char extra;
    while (1) {
        printf("Ingrese el peso en kg del empleado #%d (> 0): ", num_empleado);
        if (scanf("%lf%c", &peso, &extra) == 2 && (extra == '\n' || extra == '\r')) {
            if (peso > 0.0 && peso <= 500.0) {
                return peso;
            }
            printf("Por favor ingrese un peso valido (entre 0.1 y 500 kg).\n");
        } else {
            printf("Entrada invalida. Ingrese un numero valido.\n");
            limpiar_buffer();
        }
    }
}

static int desea_continuar(void) {
    char resp, extra;
    while (1) {
        printf("Desea ingresar otro empleado? (s/n): ");
        if (scanf(" %c%c", &resp, &extra) == 2 && (extra == '\n' || extra == '\r')) {
            resp = (char)tolower((unsigned char)resp);
            if (resp == 's') return 1;
            if (resp == 'n') return 0;
        } else {
            limpiar_buffer();
        }
        printf("Respuesta invalida. Ingrese 's' o 'n'.\n");
    }
}

int main(void) {
    int hasta_80 = 0;
    int mas_de_80 = 0;
    int n = 1;

    printf("=== EJERCICIO 25: PESO DE EMPLEADOS DE UNA FABRICA ===\n\n");

    do {
        double peso = leer_peso(n++);
        if (peso <= 80.0) {
            hasta_80++;
        } else {
            mas_de_80++;
        }
    } while (desea_continuar());

    printf("\nResumen de empleados registrados:\n");
    printf("- Empleados que pesan hasta 80 kg inclusive: %d\n", hasta_80);
    printf("- Empleados que pesan mas de 80 kg:          %d\n", mas_de_80);
    printf("- Total de empleados registrados:            %d\n\n", hasta_80 + mas_de_80);

    system("pause");
    return 0;
}
