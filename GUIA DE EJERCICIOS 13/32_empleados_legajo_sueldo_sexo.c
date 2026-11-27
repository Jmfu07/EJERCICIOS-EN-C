/*
 * Ejercicio 32:
 * Se registran de los empleados de una empresa Numero de legajo, sueldo
 * y sexo (1 femenino y 2 masculino). Disenar en Pseudocodigo y codificar en C
 * un algoritmo que permita informar cuantas mujeres ganan mas de $ 500 y
 * cuantos hombres ganan menos de $ 400.
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

static void limpiar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

static int leer_legajo(void) {
    int leg;
    char extra;
    while (1) {
        printf("Ingrese el numero de legajo (> 0): ");
        if (scanf("%d%c", &leg, &extra) == 2 && (extra == '\n' || extra == '\r')) {
            if (leg > 0) return leg;
            printf("El numero de legajo debe ser mayor que 0.\n");
        } else {
            printf("Entrada invalida. Ingrese un numero entero.\n");
            limpiar_buffer();
        }
    }
}

static double leer_sueldo(void) {
    double sueldo;
    char extra;
    while (1) {
        printf("Ingrese el sueldo ($): ");
        if (scanf("%lf%c", &sueldo, &extra) == 2 && (extra == '\n' || extra == '\r')) {
            if (sueldo > 0.0) return sueldo;
            printf("El sueldo debe ser mayor que 0.\n");
        } else {
            printf("Entrada invalida. Ingrese un monto valido.\n");
            limpiar_buffer();
        }
    }
}

static int leer_sexo(void) {
    int sexo;
    char extra;
    while (1) {
        printf("Ingrese el sexo (1 = Femenino, 2 = Masculino): ");
        if (scanf("%d%c", &sexo, &extra) == 2 && (extra == '\n' || extra == '\r')) {
            if (sexo == 1 || sexo == 2) return sexo;
            printf("Opcion invalida. Ingrese 1 o 2.\n");
        } else {
            printf("Entrada invalida. Ingrese 1 o 2.\n");
            limpiar_buffer();
        }
    }
}

static int desea_continuar(void) {
    char resp, extra;
    while (1) {
        printf("Desea registrar otro empleado? (s/n): ");
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
    int mujeres_mas_500 = 0;
    int hombres_menos_400 = 0;
    int total_empleados = 0;

    printf("=== EJERCICIO 32: REGISTRO DE EMPLEADOS (LEGAJO, SUELDO, SEXO) ===\n\n");

    do {
        int legajo = leer_legajo();
        double sueldo = leer_sueldo();
        int sexo = leer_sexo();

        (void)legajo;
        total_empleados++;

        if (sexo == 1 && sueldo > 500.0) {
            mujeres_mas_500++;
        } else if (sexo == 2 && sueldo < 400.0) {
            hombres_menos_400++;
        }
        printf("\n");
    } while (desea_continuar());

    printf("\n=== INFORME FINAL ===\n");
    printf("- Total de empleados registrados:            %d\n", total_empleados);
    printf("- Mujeres que ganan mas de $500:             %d\n", mujeres_mas_500);
    printf("- Hombres que ganan menos de $400:           %d\n\n", hombres_menos_400);

    system("pause");
    return 0;
}
