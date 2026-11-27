/*
 * Ejercicio 30 (Estructura de datos): Estructura x y f(x) = cos(x)^2 - 3x
 * -------------------------------------------------------------------
 * QUE HACE:
 *   Crea una estructura que almacena x y f(x) para la funcion:
 *     f(x) = cos(x)^2 - 3x
 *   con x perteneciente al intervalo [-10, 10] y pasos de 0.5
 *   automaticamente. Muestra todos los datos en forma de tabla.
 *
 * COMO FUNCIONA:
 *   - De -10.0 a 10.0 con paso 0.5 hay exactamente 41 puntos.
 *   - Para cada punto i (0..40), calcula x = -10.0 + i * 0.5 y
 *     fx = pow(cos(x), 2) - 3.0 * x usando <math.h>.
 *
 * TIPOS DE DATOS UTILIZADOS:
 *   - struct PuntoFuncion: double x; double fx.
 *   - int                : Indice de recorrido (0..40).
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>

#define CANT_PUNTOS 41

typedef struct {
    double x;
    double fx;
} PuntoFuncion;

double evaluarFuncion(double x) {
    double c = cos(x);
    return (c * c) - (3.0 * x);
}

int main(void) {
    PuntoFuncion tabla[CANT_PUNTOS];
    int i;

    printf("==========================================================\n");
    printf("  ESTRUCTURAS 30: TABLA DE f(x) = cos(x)^2 - 3x [-10, 10]\n");
    printf("==========================================================\n\n");

    for (i = 0; i < CANT_PUNTOS; i++) {
        tabla[i].x = -10.0 + (i * 0.5);
        tabla[i].fx = evaluarFuncion(tabla[i].x);
    }

    printf("   #  |      x      |   f(x) = cos(x)^2 - 3x\n");
    printf("------+-------------+-------------------------\n");
    for (i = 0; i < CANT_PUNTOS; i++) {
        printf("  %2d  |  %8.2f   |       %12.6f\n",
               i + 1, tabla[i].x, tabla[i].fx);
    }
    printf("------+-------------+-------------------------\n");

    printf("\nPresione cualquier tecla para finalizar...");
    _getch();
    return 0;
}
