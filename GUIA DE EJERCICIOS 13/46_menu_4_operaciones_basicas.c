/*
 * Ejercicio 46:
 * Disenar un programa en C que le presente al usuario un menu de opciones
 * con las cuatro operaciones basicas (suma, resta, multiplicacion, division).
 * Segun la operacion elegida ingresara 2 numeros enteros, se realizara la misma
 * y se mostrara por pantalla el resultado. El usuario podra trabajar en el programa
 * hasta que indique lo contrario. Recordar que la division por cero es
 * indeterminada.
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
        printf("Entrada invalida. Ingrese un numero entero.\n");
        limpiar_buffer();
    }
}

static int leer_opcion(void) {
    int op;
    while (1) {
        op = leer_entero("Seleccione una opcion (1-5): ");
        if (op >= 1 && op <= 5) return op;
        printf("Opcion fuera de rango. Elija entre 1 y 5.\n");
    }
}

int main(void) {
    int opcion;

    printf("=== EJERCICIO 46: MENU DE 4 OPERACIONES BASICAS ===\n\n");

    while (1) {
        int a, b;

        printf("---------------- MENU ----------------\n");
        printf("1. Suma\n");
        printf("2. Resta\n");
        printf("3. Multiplicacion\n");
        printf("4. Division\n");
        printf("5. Salir\n");
        printf("--------------------------------------\n");

        opcion = leer_opcion();
        if (opcion == 5) {
            printf("\nSaliendo del programa...\n\n");
            break;
        }

        a = leer_entero("Ingrese el primer numero entero : ");
        b = leer_entero("Ingrese el segundo numero entero: ");

        switch (opcion) {
            case 1:
                printf("\nResultado: %d + %d = %d\n\n", a, b, a + b);
                break;
            case 2:
                printf("\nResultado: %d - %d = %d\n\n", a, b, a - b);
                break;
            case 3:
                printf("\nResultado: %d * %d = %lld\n\n", a, b, (long long)a * b);
                break;
            case 4:
                if (b == 0) {
                    printf("\nERROR: La division por cero es indeterminada.\n\n");
                } else {
                    printf("\nResultado: %d / %d = %.4f (Cociente entero: %lld, Resto: %lld)\n\n",
                           a, b, (double)a / b, (long long)a / b, (long long)a % b);
                }
                break;
            default:
                break;
        }
    }

    system("pause");
    return 0;
}
