/*
 * Ejercicio 18:
 * Ingresar un numero, entero y efectuar la suma de todos los numeros que
 * le anteceden, comenzando desde 0 y mostrar el resultado por pantalla.
 */

#include <stdio.h>
#include <stdlib.h>

static void limpiar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
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
            printf("Por favor ingrese un numero entero mayor o igual a 0.\n");
        } else {
            printf("Entrada invalida. Ingrese un numero entero valido.\n");
            limpiar_buffer();
        }
    }
}

int main(void) {
    int n, i;
    long long suma = 0;

    printf("=== EJERCICIO 18: SUMA DE LOS NUMEROS ANTECEDENTES DESDE 0 ===\n\n");

    n = leer_entero_no_negativo("Ingrese un numero entero (>= 0): ");

    for (i = 0; i < n; i++) {
        suma += i;
    }

    printf("\nLa suma de los numeros que anteceden a %d (de 0 a %d) es: %lld\n\n",
           n, (n > 0 ? n - 1 : 0), suma);

    system("pause");
    return 0;
}
