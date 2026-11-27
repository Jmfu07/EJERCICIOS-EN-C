/*
 * Ejercicio 26:
 * En una tienda de articulos para caballeros al final del dia se carga en la
 * computadora las boletas que confeccionaron los distintos vendedores para
 * saber cuanto fue la comision del dia de cada uno de ellos. Los datos que se
 * ingresan (por boleta) son: el numero de vendedor y el importe. Cuando no hay
 * mas boletas para cargar se ingresa 0. Teniendo en cuenta que el negocio tiene
 * 3 vendedores y que el porcentaje sobre las ventas es del 5%, indicar cuanto
 * gano cada vendedor en el dia.
 */

#include <stdio.h>
#include <stdlib.h>

static void limpiar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

static int leer_vendedor(void) {
    int vend;
    char extra;
    while (1) {
        printf("Ingrese el numero de vendedor (1, 2, 3 o 0 para terminar): ");
        if (scanf("%d%c", &vend, &extra) == 2 && (extra == '\n' || extra == '\r')) {
            if (vend >= 0 && vend <= 3) {
                return vend;
            }
            printf("Vendedor invalido. Debe ser 1, 2, 3 o 0 para salir.\n");
        } else {
            printf("Entrada invalida. Ingrese un numero entero.\n");
            limpiar_buffer();
        }
    }
}

static double leer_importe(void) {
    double imp;
    char extra;
    while (1) {
        printf("Ingrese el importe de la boleta (> 0): $");
        if (scanf("%lf%c", &imp, &extra) == 2 && (extra == '\n' || extra == '\r')) {
            if (imp > 0.0) {
                return imp;
            }
            printf("El importe debe ser mayor que cero.\n");
        } else {
            printf("Entrada invalida. Ingrese un importe numerico.\n");
            limpiar_buffer();
        }
    }
}

int main(void) {
    double ventas[3] = {0.0, 0.0, 0.0};
    int boletas[3] = {0, 0, 0};
    int i;

    printf("=== EJERCICIO 26: COMISION DEL 5%% DE 3 VENDEDORES ===\n\n");

    while (1) {
        int vendedor = leer_vendedor();
        double importe;
        if (vendedor == 0) {
            break;
        }
        importe = leer_importe();
        ventas[vendedor - 1] += importe;
        boletas[vendedor - 1]++;
        printf("Boleta registrada para el vendedor %d.\n\n", vendedor);
    }

    printf("\n=== INFORME DEL DIA ===\n");
    for (i = 0; i < 3; i++) {
        double comision = ventas[i] * 0.05;
        printf("Vendedor %d: %d boleta(s) | Ventas totales: $%.2f | Comision (5%%): $%.2f\n",
               i + 1, boletas[i], ventas[i], comision);
    }
    printf("\n");

    system("pause");
    return 0;
}
