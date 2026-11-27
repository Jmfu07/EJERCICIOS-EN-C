/*
 * Ejercicio 30:
 * Realizar la tabla de multiplicar de un numero entre 0 y 10 de forma que se
 * visualice de la siguiente forma:
 * 4x1= 4
 * 4x2= 8
 * ..........
 */

#include <stdio.h>
#include <stdlib.h>

static void limpiar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

static int leer_numero_0_a_10(void) {
    int n;
    char extra;
    while (1) {
        printf("Ingrese un numero entero entre 0 y 10: ");
        if (scanf("%d%c", &n, &extra) == 2 && (extra == '\n' || extra == '\r')) {
            if (n >= 0 && n <= 10) {
                return n;
            }
            printf("Error: el numero debe estar comprendido entre 0 y 10.\n");
        } else {
            printf("Entrada invalida. Ingrese un numero entero.\n");
            limpiar_buffer();
        }
    }
}

int main(void) {
    int n, i;

    printf("=== EJERCICIO 30: TABLA DE MULTIPLICAR (0 A 10) ===\n\n");

    n = leer_numero_0_a_10();

    printf("\nTabla del %d:\n", n);
    for (i = 1; i <= 10; i++) {
        printf("%dx%d= %d\n", n, i, n * i);
    }
    printf("\n");

    system("pause");
    return 0;
}
