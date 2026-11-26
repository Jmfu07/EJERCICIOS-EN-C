/*
 * Ejercicio 31:
 * Hacer un programa en C que luego de ingresar 2 numeros por teclado,
 * imprima los numeros naturales que hay entre ambos empezando por el mas
 * pequeno, contar cuantos numeros hay y cuantos de ellos son pares.
 */

#include <stdio.h>
#include <stdlib.h>

static void limpiar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

static int leer_entero(const char *mensaje) {
    int valor;
    char extra;
    while (1) {
        printf("%s", mensaje);
        if (scanf("%d%c", &valor, &extra) == 2 && (extra == '\n' || extra == '\r')) {
            return valor;
        }
        printf("Entrada invalida. Ingrese un numero entero valido.\n");
        limpiar_buffer();
    }
}

int main(void) {
    int n1, n2, menor, mayor, i;
    int cant_naturales = 0, cant_pares = 0;

    printf("=== EJERCICIO 31: NATURALES ENTRE DOS NUMEROS ===\n\n");

    n1 = leer_entero("Ingrese el primer numero entero: ");
    n2 = leer_entero("Ingrese el segundo numero entero: ");

    menor = (n1 < n2) ? n1 : n2;
    mayor = (n1 > n2) ? n1 : n2;

    printf("\nNumeros naturales entre %d y %d (inclusive):\n", menor, mayor);
    for (i = menor; i <= mayor; i++) {
        if (i >= 1) {
            printf("%4d ", i);
            cant_naturales++;
            if (i % 2 == 0) {
                cant_pares++;
            }
            if (cant_naturales % 10 == 0) {
                printf("\n");
            }
        }
    }

    if (cant_naturales == 0) {
        printf("(No hay numeros naturales en ese rango)");
    }

    printf("\n\nResultados:\n");
    printf("- Cantidad total de numeros naturales en el rango: %d\n", cant_naturales);
    printf("- Cantidad de numeros naturales pares en el rango: %d\n\n", cant_pares);

    system("pause");
    return 0;
}
