/*
 * Ejercicio 29:
 * Por teclado se ingresa el valor hora de un empleado. Posteriormente se
 * ingresa el nombre del empleado, la antiguedad y la cantidad de horas
 * trabajadas en el mes. Se pide calcular el importe a cobrar teniendo en cuenta
 * que al total que resulta de multiplicar el valor hora por la cantidad de horas
 * trabajadas, hay que sumarle la cantidad de anos trabajados multiplicados por
 * $30, y al total de todas esas operaciones restarle el 13% en concepto de
 * descuentos. Imprimir el recibo correspondiente con el nombre, la antiguedad, el
 * valor hora, el total a cobrar en bruto, el total de descuentos y el valor neto a
 * cobrar.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void limpiar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

static double leer_double_positivo(const char *mensaje) {
    double valor;
    char extra;
    while (1) {
        printf("%s", mensaje);
        if (scanf("%lf%c", &valor, &extra) == 2 && (extra == '\n' || extra == '\r')) {
            if (valor > 0.0) {
                return valor;
            }
            printf("El valor debe ser mayor que cero.\n");
        } else {
            printf("Entrada invalida. Ingrese un numero valido.\n");
            limpiar_buffer();
        }
    }
}

static int leer_entero_no_negativo(const char *mensaje) {
    int valor;
    char extra;
    while (1) {
        printf("%s", mensaje);
        if (scanf("%d%c", &valor, &extra) == 2 && (extra == '\n' || extra == '\r')) {
            if (valor >= 0) {
                return valor;
            }
            printf("El valor no puede ser negativo.\n");
        } else {
            printf("Entrada invalida. Ingrese un numero entero valido.\n");
            limpiar_buffer();
        }
    }
}

static void leer_texto(const char *mensaje, char *destino, size_t tam) {
    while (1) {
        printf("%s", mensaje);
        if (fgets(destino, (int)tam, stdin) != NULL) {
            size_t len = strlen(destino);
            while (len > 0 && (destino[len - 1] == '\n' || destino[len - 1] == '\r')) {
                destino[--len] = '\0';
            }
            if (len > 0) {
                return;
            }
        }
        printf("El texto no puede estar vacio.\n");
    }
}

int main(void) {
    char nombre[100];
    double valor_hora, horas_trabajadas, bruto, descuentos, neto;
    int antiguedad;

    printf("=== EJERCICIO 29: RECIBO DE SUELDO CON ANTIGUEDAD ===\n\n");

    valor_hora = leer_double_positivo("Ingrese el valor hora del empleado ($): ");
    leer_texto("Ingrese el nombre del empleado: ", nombre, sizeof(nombre));
    antiguedad = leer_entero_no_negativo("Ingrese la antiguedad en anos (>= 0): ");
    horas_trabajadas = leer_double_positivo("Ingrese la cantidad de horas trabajadas en el mes: ");

    bruto = (valor_hora * horas_trabajadas) + (antiguedad * 30.0);
    descuentos = bruto * 0.13;
    neto = bruto - descuentos;

    printf("\n============================================\n");
    printf("              RECIBO DE SUELDO              \n");
    printf("============================================\n");
    printf("Nombre del empleado : %s\n", nombre);
    printf("Antiguedad          : %d ano(s)\n", antiguedad);
    printf("Valor hora          : $%.2f\n", valor_hora);
    printf("Horas trabajadas    : %.2f\n", horas_trabajadas);
    printf("--------------------------------------------\n");
    printf("Total a cobrar bruto: $%.2f\n", bruto);
    printf("Total descuentos 13%%: $%.2f\n", descuentos);
    printf("Valor neto a cobrar : $%.2f\n", neto);
    printf("============================================\n\n");

    system("pause");
    return 0;
}
