/*
 * Ejercicio 31 (Estructura de datos): Estructura x y f(x) al azar ordenado descendente
 * -------------------------------------------------------------------
 * QUE HACE:
 *   Almacena en un arreglo de N estructuras los valores de x y
 *   f(x) = cos(x)^2 - 3x, donde cada x se genera al azar en el rango
 *   [-10, 10] con un digito decimal (del 0 al 9). Luego ordena los N
 *   registros de modo descendente por X y muestra los datos.
 *
 * COMO FUNCIONA:
 *   - Para obtener un numero aleatorio en [-10.0, 10.0] con 1 decimal,
 *     genera un entero aleatorio entre -100 y 100 ((-100 + rand() % 201))
 *     y lo divide por 10.0.
 *   - Evalua f(x) = cos(x)^2 - 3x con <math.h>.
 *   - Ordena el arreglo de estructuras de mayor a menor X (descendente).
 *
 * TIPOS DE DATOS UTILIZADOS:
 *   - struct PuntoFuncion: double x; double fx.
 *   - int                : Cantidad de registros N e indices.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <conio.h>

#define MAX_REG 100

typedef struct {
    double x;
    double fx;
} PuntoFuncion;

int leerEnteroRango(const char *mensaje, int min, int max) {
    char buf[16];
    int idx, c, val;
    while (1) {
        printf("%s (%d a %d): ", mensaje, min, max);
        idx = 0;
        while (1) {
            c = _getch();
            if (c == 0 || c == 224) {
                _getch();
                continue;
            }
            if (c == '\r' && idx > 0) {
                buf[idx] = '\0';
                printf("\n");
                break;
            } else if (c == '\b' && idx > 0) {
                idx--;
                printf("\b \b");
            } else if (c >= '0' && c <= '9' && idx < 9) {
                buf[idx++] = (char)c;
                printf("%c", c);
            }
        }
        val = atoi(buf);
        if (val >= min && val <= max) {
            return val;
        }
        printf("   [!] Error: ingrese un numero entre %d y %d.\n", min, max);
    }
}

double evaluarFuncion(double x) {
    double c = cos(x);
    return (c * c) - (3.0 * x);
}

void ordenarDescendentePorX(PuntoFuncion v[], int n) {
    int i, j;
    PuntoFuncion aux;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if (v[j].x < v[j + 1].x) {
                aux = v[j];
                v[j] = v[j + 1];
                v[j + 1] = aux;
            }
        }
    }
}

void mostrarTabla(const char *titulo, const PuntoFuncion v[], int n) {
    int i;
    printf("\n%s:\n", titulo);
    printf("   #  |     X      |   f(X) = cos(X)^2 - 3X\n");
    printf("------+------------+-------------------------\n");
    for (i = 0; i < n; i++) {
        printf("  %2d  |  %7.1f   |       %12.6f\n",
               i + 1, v[i].x, v[i].fx);
    }
    printf("------+------------+-------------------------\n");
}

int main(void) {
    PuntoFuncion reg[MAX_REG];
    int n, i, decimas;

    srand((unsigned int)time(NULL));

    printf("==============================================================\n");
    printf("  ESTRUCTURAS 31: f(x) = cos(x)^2 - 3x AL AZAR Y ORDENADO\n");
    printf("==============================================================\n\n");

    n = leerEnteroRango("Ingrese la cantidad N de registros a generar", 1, MAX_REG);

    for (i = 0; i < n; i++) {
        decimas = -100 + (rand() % 201); /* -100 a +100 -> -10.0 a +10.0 */
        reg[i].x = (double)decimas / 10.0;
        reg[i].fx = evaluarFuncion(reg[i].x);
    }

    mostrarTabla("Registros generados al azar", reg, n);

    ordenarDescendentePorX(reg, n);
    mostrarTabla("Registros ordenados de modo DESCENDENTE por X", reg, n);

    printf("\nPresione cualquier tecla para finalizar...");
    _getch();
    return 0;
}
