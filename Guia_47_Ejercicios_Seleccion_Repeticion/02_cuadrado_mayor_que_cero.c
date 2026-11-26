/*
 * Ejercicio 2:
 * Escribir el programa en C necesario para calcular y mostrar el cuadrado de
 * un numero. El numero debe ser mayor que cero, en caso de error que aparezca
 * el mensaje "ERROR, el numero debe ser mayor que cero".
 */

#include <stdio.h>
#include <stdlib.h>

static void limpiar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

static double leer_double(const char *mensaje) {
    double valor;
    char extra;
    while (1) {
        printf("%s", mensaje);
        if (scanf("%lf%c", &valor, &extra) == 2 && (extra == '\n' || extra == '\r')) {
            return valor;
        }
        printf("Entrada invalida. Por favor ingrese un numero valido.\n");
        limpiar_buffer();
    }
}

int main(void) {
    double numero;

    printf("=== EJERCICIO 2: CUADRADO DE UN NUMERO MAYOR QUE CERO ===\n\n");

    while (1) {
        numero = leer_double("Ingrese un numero (> 0): ");
        if (numero <= 0.0) {
            printf("ERROR, el numero debe ser mayor que cero\n\n");
        } else {
            break;
        }
    }

    printf("\nEl cuadrado de %.2f es: %.2f\n\n", numero, numero * numero);

    system("pause");
    return 0;
}
