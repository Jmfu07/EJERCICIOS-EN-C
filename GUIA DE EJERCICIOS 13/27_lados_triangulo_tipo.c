/*
 * Ejercicio 27:
 * Ingresar por teclado 3 numeros correspondientes a los lados de un
 * triangulo. Teniendo en cuenta que la suma de los dos lados menores tiene que
 * ser superior al lado mayor para que formen un triangulo, indicar si los numeros
 * indicados forman un triangulo y si lo forman que tipo de triangulo es (segun sus
 * lados).
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define EPS 1e-9

static void limpiar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

static double leer_lado(const char *mensaje) {
    double valor;
    char extra;
    while (1) {
        printf("%s", mensaje);
        if (scanf("%lf%c", &valor, &extra) == 2 && (extra == '\n' || extra == '\r')) {
            if (valor > 0.0) {
                return valor;
            }
            printf("La longitud del lado debe ser mayor que cero.\n");
        } else {
            printf("Entrada invalida. Ingrese un numero valido.\n");
            limpiar_buffer();
        }
    }
}

static int iguales(double x, double y) {
    return fabs(x - y) < EPS;
}

int main(void) {
    double a, b, c;
    double mayor, suma_menores;

    printf("=== EJERCICIO 27: VERIFICACION Y CLASIFICACION DE UN TRIANGULO ===\n\n");

    a = leer_lado("Ingrese el lado 1 (> 0): ");
    b = leer_lado("Ingrese el lado 2 (> 0): ");
    c = leer_lado("Ingrese el lado 3 (> 0): ");

    mayor = a;
    if (b > mayor) mayor = b;
    if (c > mayor) mayor = c;
    suma_menores = (a + b + c) - mayor;

    if (suma_menores > mayor + EPS) {
        printf("\nLos lados %.2f, %.2f y %.2f SI forman un triangulo.\n", a, b, c);
        if (iguales(a, b) && iguales(b, c)) {
            printf("Tipo de triangulo: EQUILATERO (los 3 lados iguales).\n\n");
        } else if (iguales(a, b) || iguales(a, c) || iguales(b, c)) {
            printf("Tipo de triangulo: ISOSCELES (2 lados iguales).\n\n");
        } else {
            printf("Tipo de triangulo: ESCALENO (los 3 lados distintos).\n\n");
        }
    } else {
        printf("\nLos lados %.2f, %.2f y %.2f NO forman un triangulo\n", a, b, c);
        printf("(la suma de los dos menores %.2f no supera al lado mayor %.2f).\n\n",
               suma_menores, mayor);
    }

    system("pause");
    return 0;
}
