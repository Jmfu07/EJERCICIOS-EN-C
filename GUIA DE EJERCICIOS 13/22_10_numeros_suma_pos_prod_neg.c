/*
 * Ejercicio 22:
 * Disenar en Pseudocodigo y codificar en C un algoritmo que permita
 * ingresar 10 numeros, ninguno de ellos igual a cero. Se pide sumar los positivos,
 * obtener el producto de los negativos y luego mostrar ambos resultados.
 */

#include <stdio.h>
#include <stdlib.h>

static void limpiar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

static double leer_double_distinto_de_cero(int indice) {
    double valor;
    char extra;
    while (1) {
        printf("Ingrese el numero %2d de 10 (distinto de 0): ", indice);
        if (scanf("%lf%c", &valor, &extra) == 2 && (extra == '\n' || extra == '\r')) {
            if (valor != 0.0) {
                return valor;
            }
            printf("Error: el numero no puede ser igual a cero.\n");
        } else {
            printf("Entrada invalida. Ingrese un numero valido.\n");
            limpiar_buffer();
        }
    }
}

int main(void) {
    double suma_positivos = 0.0;
    double prod_negativos = 1.0;
    int cant_positivos = 0, cant_negativos = 0;
    int i;

    printf("=== EJERCICIO 22: SUMA DE POSITIVOS Y PRODUCTO DE NEGATIVOS ===\n\n");

    for (i = 1; i <= 10; i++) {
        double num = leer_double_distinto_de_cero(i);
        if (num > 0.0) {
            suma_positivos += num;
            cant_positivos++;
        } else {
            prod_negativos *= num;
            cant_negativos++;
        }
    }

    printf("\nResultados:\n");
    printf("- Suma de los numeros positivos (%d ingresados): %.2f\n",
           cant_positivos, suma_positivos);
    if (cant_negativos > 0) {
        printf("- Producto de los numeros negativos (%d ingresados): %.2f\n\n",
               cant_negativos, prod_negativos);
    } else {
        printf("- No se ingresaron numeros negativos (producto = 0.00).\n\n");
    }

    system("pause");
    return 0;
}
