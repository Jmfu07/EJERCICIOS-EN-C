/*
 * Ejercicio 44:
 * Escribir un programa en C que determine si un ano es bisiesto. Un ano es
 * bisiesto si es multiplo de 4. Los anos multiplos de 100 no son bisiestos, salvo si
 * ellos tambien son multiplos de 400 (2000 es bisiesto pero 1900 no).
 */

#include <stdio.h>
#include <stdlib.h>

static void limpiar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

static int leer_ano(void) {
    int ano;
    char extra;
    while (1) {
        printf("Ingrese un ano (> 0): ");
        if (scanf("%d%c", &ano, &extra) == 2 && (extra == '\n' || extra == '\r')) {
            if (ano > 0) return ano;
            printf("El ano debe ser mayor que cero.\n");
        } else {
            printf("Entrada invalida. Ingrese un ano entero.\n");
            limpiar_buffer();
        }
    }
}

static int es_bisiesto(int ano) {
    return (ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0);
}

int main(void) {
    int ano;

    printf("=== EJERCICIO 44: DETERMINAR SI UN ANO ES BISIESTO ===\n\n");

    ano = leer_ano();

    if (es_bisiesto(ano)) {
        printf("\nEl ano %d ES BISIESTO.\n\n", ano);
    } else {
        printf("\nEl ano %d NO es bisiesto.\n\n", ano);
    }

    system("pause");
    return 0;
}
