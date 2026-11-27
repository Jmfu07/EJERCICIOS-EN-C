/*
 * Ejercicio 24:
 * Escribir un programa en C que visualice una tabla de los N primeros
 * numeros, siendo N un numero que ingresa el usuario. Utilizar el siguiente
 * diseno de salida suponiendo que el usuario ingreso un tres:
 * NUMERO   CUADRADO   CUBO
 *   1         1         1
 *   2         4         8
 *   3         9        27
 */

#include <stdio.h>
#include <stdlib.h>

static void limpiar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

static int leer_natural(const char *mensaje) {
    int valor;
    char extra;
    while (1) {
        printf("%s", mensaje);
        if (scanf("%d%c", &valor, &extra) == 2 && (extra == '\n' || extra == '\r')) {
            if (valor >= 1 && valor <= 10000) {
                return valor;
            }
            printf("Por favor ingrese un numero entero entre 1 y 10000.\n");
        } else {
            printf("Entrada invalida. Ingrese un numero entero valido.\n");
            limpiar_buffer();
        }
    }
}

int main(void) {
    int n, i;

    printf("=== EJERCICIO 24: TABLA DE NUMERO, CUADRADO Y CUBO ===\n\n");

    n = leer_natural("Ingrese la cantidad de numeros N (>= 1): ");

    printf("\n%-10s %-12s %-12s\n", "NUMERO", "CUADRADO", "CUBO");
    for (i = 1; i <= n; i++) {
        long long cuadrado = (long long)i * i;
        long long cubo = cuadrado * i;
        printf("%-10d %-12lld %-12lld\n", i, cuadrado, cubo);
    }
    printf("\n");

    system("pause");
    return 0;
}
