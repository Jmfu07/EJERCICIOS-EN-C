/*
 * Ejercicio 11:
 * Realizar un programa en C que permita ingresar un numero correspondiente
 * a los dias de una semana y muestre el nombre del dia. Que se permita trabajar
 * hasta que el usuario indique lo contrario.
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

static void limpiar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

static int leer_dia(void) {
    int dia;
    char extra;
    while (1) {
        printf("Ingrese un numero de dia de la semana (1 al 7): ");
        if (scanf("%d%c", &dia, &extra) == 2 && (extra == '\n' || extra == '\r')) {
            if (dia >= 1 && dia <= 7) {
                return dia;
            }
            printf("El numero debe estar entre 1 y 7.\n");
        } else {
            printf("Entrada invalida. Ingrese un numero entero del 1 al 7.\n");
            limpiar_buffer();
        }
    }
}

static int desea_continuar(void) {
    char resp, extra;
    while (1) {
        printf("Desea consultar otro dia? (s/n): ");
        if (scanf(" %c%c", &resp, &extra) == 2 && (extra == '\n' || extra == '\r')) {
            resp = (char)tolower((unsigned char)resp);
            if (resp == 's') return 1;
            if (resp == 'n') return 0;
        } else {
            limpiar_buffer();
        }
        printf("Respuesta invalida. Ingrese 's' para si o 'n' para no.\n");
    }
}

int main(void) {
    const char *dias[] = {
        "Lunes", "Martes", "Miercoles", "Jueves", "Viernes", "Sabado", "Domingo"
    };

    printf("=== EJERCICIO 11: DIA DE LA SEMANA ===\n\n");

    do {
        int dia = leer_dia();
        printf("El dia %d corresponde a: %s\n\n", dia, dias[dia - 1]);
    } while (desea_continuar());

    printf("\nFin del programa.\n\n");
    system("pause");
    return 0;
}
