/*
 * Ejercicio 47:
 * La fecha de Pascua corresponde al primer domingo despues de la primera
 * luna llena que sigue al equinoccio de primavera, y se calcula con las siguientes
 * expresiones:
 * A = ano mod 19
 * B = ano mod 4
 * C = ano mod 7
 * D = (19*A + 24) mod 30
 * E = (2*B + 4*C + 6*D + 5) mod 7
 * N = (22 + D + E)
 * En el que N indica el numero del dia de marzo (o Abril si N > 31)
 * correspondiente al domingo de Pascua. Realizar un programa que calcule esa
 * fecha para los anos 2003 y 2004 (y permita consultar otro ano).
 */

#include <stdio.h>
#include <stdlib.h>

static void limpiar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

static int leer_ano_opcional(void) {
    int ano;
    char extra;
    while (1) {
        printf("Ingrese un ano Gregoriano >= 1583 para calcular Pascua (o 0 para terminar): ");
        if (scanf("%d%c", &ano, &extra) == 2 && (extra == '\n' || extra == '\r')) {
            if (ano == 0 || (ano >= 1583 && ano <= 9999)) return ano;
            printf("Ingrese 0 para salir o un ano entre 1583 y 9999.\n");
        } else {
            printf("Entrada invalida. Ingrese un numero entero.\n");
            limpiar_buffer();
        }
    }
}

static void mostrar_pascua(int ano) {
    int a = ano % 19, b = ano / 100, c = ano % 100;
    int d = b / 4, e = b % 4, f = (b + 8) / 25, g = (b - f + 1) / 3;
    int h = (19 * a + b - d - g + 15) % 30;
    int i = c / 4, k = c % 4, l = (32 + 2 * e + 2 * i - h - k) % 7;
    int m = (a + 11 * h + 22 * l) / 451;
    int mes = (h + l - 7 * m + 114) / 31;
    int dia = ((h + l - 7 * m + 114) % 31) + 1;

    printf("Ano %d -> Domingo de Pascua: %d de %s\n", ano, dia, (mes == 3) ? "Marzo" : "Abril");
}

int main(void) {
    int ano;

    printf("=== EJERCICIO 47: CALCULO DE LA FECHA DE PASCUA ===\n\n");

    printf("Fechas solicitadas por el enunciado:\n");
    mostrar_pascua(2003);
    mostrar_pascua(2004);
    printf("\n");

    while (1) {
        ano = leer_ano_opcional();
        if (ano == 0) {
            break;
        }
        mostrar_pascua(ano);
        printf("\n");
    }

    printf("\n");
    system("pause");
    return 0;
}
