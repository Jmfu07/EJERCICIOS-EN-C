/*
 * Ejercicio 9:
 * Dados ciertos centimetros como entrada de tipo decimal, escribir su
 * equivalente a pies (enteros) y pulgadas (decimal), dando las pulgadas con una
 * precision de 1 lugar decimal. Considerar 2.54 cm por pulgada y 12 pulgadas por
 * pie. La salida podra ser: 333.3 cm son 10 pies y 11.2 pulgadas.
 */

#include <stdio.h>
#include <stdlib.h>

static void limpiar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

static double leer_double_no_negativo(const char *mensaje) {
    double valor;
    char extra;
    while (1) {
        printf("%s", mensaje);
        if (scanf("%lf%c", &valor, &extra) == 2 && (extra == '\n' || extra == '\r')) {
            if (valor >= 0.0) {
                return valor;
            }
            printf("La medida en centimetros no puede ser negativa.\n");
        } else {
            printf("Entrada invalida. Por favor ingrese un numero valido.\n");
            limpiar_buffer();
        }
    }
}

int main(void) {
    double cm, total_pulgadas, pulgadas_restantes;
    int pies;

    printf("=== EJERCICIO 9: CENTIMETROS A PIES Y PULGADAS ===\n\n");

    cm = leer_double_no_negativo("Ingrese la cantidad de centimetros (>= 0): ");
    total_pulgadas = cm / 2.54;
    pies = (int)(total_pulgadas / 12.0);
    pulgadas_restantes = total_pulgadas - (pies * 12.0);

    printf("\n%.1f cm son %d pies y %.1f pulgadas.\n\n", cm, pies, pulgadas_restantes);

    system("pause");
    return 0;
}
