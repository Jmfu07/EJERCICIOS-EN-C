/*
 * Ejercicios 39, 40, 41 y 42 (integrados):
 * 39. Escribir un programa en C que calcule el salario semanal de cada
 *     trabajador de una empresa dada la tarifa horaria y el numero de horas
 *     trabajadas ademas del nombre. Se debe permitir al usuario indicar cuando
 *     desea dejar de ingresar datos.
 * 40. Idem al anterior pero informando al finalizar la cantidad de empleados
 *     a los que se les calculo el salario.
 * 41. Idem al anterior agregando en el informe final nombre del empleado de
 *     mayor salario con el monto correspondiente y nombre del empleado de
 *     menor salario con el monto correspondiente.
 * 42. Idem al anterior agregando en el informe final el costo de la empresa en
 *     salarios y el salario promedio.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static void limpiar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

static void leer_texto(const char *mensaje, char *destino, size_t tam) {
    while (1) {
        printf("%s", mensaje);
        if (fgets(destino, (int)tam, stdin) != NULL) {
            size_t len = strlen(destino);
            while (len > 0 && (destino[len - 1] == '\n' || destino[len - 1] == '\r')) {
                destino[--len] = '\0';
            }
            if (len > 0) return;
        }
        printf("El nombre no puede estar vacio.\n");
    }
}

static double leer_double_positivo(const char *mensaje) {
    double valor;
    char extra;
    while (1) {
        printf("%s", mensaje);
        if (scanf("%lf%c", &valor, &extra) == 2 && (extra == '\n' || extra == '\r')) {
            if (valor > 0.0) return valor;
            printf("El valor debe ser mayor que cero.\n");
        } else {
            printf("Entrada invalida. Ingrese un numero valido.\n");
            limpiar_buffer();
        }
    }
}

static int desea_continuar(void) {
    char resp, extra;
    while (1) {
        printf("Desea ingresar otro trabajador? (s/n): ");
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
    char nombre[100];
    char nombre_mayor[100] = "", nombre_menor[100] = "";
    double mayor_salario = 0.0, menor_salario = 0.0;
    double costo_total = 0.0;
    int cantidad_empleados = 0;

    printf("=== EJERCICIOS 39, 40, 41 Y 42: SALARIO SEMANAL E INFORME COMPLETO ===\n\n");

    do {
        double tarifa, horas, salario;

        printf("--- Trabajador #%d ---\n", cantidad_empleados + 1);
        leer_texto("Nombre del trabajador: ", nombre, sizeof(nombre));
        tarifa = leer_double_positivo("Tarifa horaria ($): ");
        horas = leer_double_positivo("Horas trabajadas en la semana: ");

        salario = tarifa * horas;
        printf("-> Salario semanal de %s: $%.2f\n\n", nombre, salario);

        if (cantidad_empleados == 0 || salario > mayor_salario) {
            mayor_salario = salario;
            strncpy(nombre_mayor, nombre, sizeof(nombre_mayor) - 1);
            nombre_mayor[sizeof(nombre_mayor) - 1] = '\0';
        }
        if (cantidad_empleados == 0 || salario < menor_salario) {
            menor_salario = salario;
            strncpy(nombre_menor, nombre, sizeof(nombre_menor) - 1);
            nombre_menor[sizeof(nombre_menor) - 1] = '\0';
        }

        costo_total += salario;
        cantidad_empleados++;
    } while (desea_continuar());

    printf("\n======================================================\n");
    printf("                INFORME FINAL DE SALARIOS             \n");
    printf("======================================================\n");
    printf("Cantidad de empleados procesados : %d\n", cantidad_empleados);
    printf("Empleado de MAYOR salario        : %s ($%.2f)\n", nombre_mayor, mayor_salario);
    printf("Empleado de MENOR salario        : %s ($%.2f)\n", nombre_menor, menor_salario);
    printf("Costo total de la empresa        : $%.2f\n", costo_total);
    printf("Salario promedio                 : $%.2f\n", costo_total / cantidad_empleados);
    printf("======================================================\n\n");

    system("pause");
    return 0;
}
