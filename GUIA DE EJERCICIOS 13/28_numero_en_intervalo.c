/*
 * Ejercicio 28:
 * Dados 3 numeros donde el primero y el ultimo son limites de un intervalo,
 * indicar si el tercero pertenece a dicho intervalo.
 */

#include <stdio.h>
#include <stdlib.h>

static void limpiar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

static double leer_double(const char *mensaje) {
    double valor;
    char extra;
    while (1) {
        printf("%s", mensaje);
        if (scanf("%lf%c", &valor, &extra) == 2 && (extra == '\n' || extra == '\r')) {
            return valor;
        }
        printf("Entrada invalida. Por favor ingrese un numero valido.\n");
        limpiar_buffer();
    }
}

int main(void) {
    double n1, n2, n3;
    double lim_inf, lim_sup;

    printf("=== EJERCICIO 28: PERTENENCIA A UN INTERVALO ===\n\n");

    n1 = leer_double("Ingrese el primer numero (limite del intervalo): ");
    n2 = leer_double("Ingrese el segundo numero (otro limite o valor a evaluar): ");
    n3 = leer_double("Ingrese el tercer numero: ");

    lim_inf = (n1 < n3) ? n1 : n3;
    lim_sup = (n1 > n3) ? n1 : n3;

    printf("\nInterpretacion 1 (1ro=%.2f y 3ro=%.2f son limites, se evalua el 2do=%.2f):\n",
           n1, n3, n2);
    if (n2 >= lim_inf && n2 <= lim_sup) {
        printf("-> El numero %.2f PERTENECE al intervalo [%.2f, %.2f].\n", n2, lim_inf, lim_sup);
    } else {
        printf("-> El numero %.2f NO pertenece al intervalo [%.2f, %.2f].\n", n2, lim_inf, lim_sup);
    }

    lim_inf = (n1 < n2) ? n1 : n2;
    lim_sup = (n1 > n2) ? n1 : n2;
    printf("\nInterpretacion 2 (1ro=%.2f y 2do=%.2f son limites, se evalua el 3ro=%.2f):\n",
           n1, n2, n3);
    if (n3 >= lim_inf && n3 <= lim_sup) {
        printf("-> El numero %.2f PERTENECE al intervalo [%.2f, %.2f].\n\n", n3, lim_inf, lim_sup);
    } else {
        printf("-> El numero %.2f NO pertenece al intervalo [%.2f, %.2f].\n\n", n3, lim_inf, lim_sup);
    }

    system("pause");
    return 0;
}
