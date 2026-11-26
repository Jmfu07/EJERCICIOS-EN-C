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
        printf("Ingrese un ano para calcular Pascua (o 0 para terminar): ");
        if (scanf("%d%c", &ano, &extra) == 2 && (extra == '\n' || extra == '\r')) {
            if (ano >= 0) return ano;
            printf("El ano no puede ser negativo.\n");
        } else {
            printf("Entrada invalida. Ingrese un numero entero.\n");
            limpiar_buffer();
        }
    }
}

static void mostrar_pascua(int ano) {
    int A = ano % 19;
    int B = ano % 4;
    int C = ano % 7;
    int D = (19 * A + 24) % 30;
    int E = (2 * B + 4 * C + 6 * D + 5) % 7;
    int N = 22 + D + E;

    if (N <= 31) {
        printf("Ano %d -> Domingo de Pascua: %d de Marzo (N = %d)\n", ano, N, N);
    } else {
        printf("Ano %d -> Domingo de Pascua: %d de Abril (N = %d)\n", ano, N - 31, N);
    }
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
