/*
 * Ejercicio 13 (Funciones y procedimientos): Tres vectores de 30, vector promedio y maximo
 * -------------------------------------------------------------------
 * QUE HACE:
 *   Genera tres vectores v1, v2 y v3 con 30 numeros enteros al azar del
 *   1 al 10. En otro vector "p" de 30 elementos reales almacena el promedio
 *   de cada posicion de los tres vectores. Muestra el vector "p" y su
 *   valor maximo.
 *
 * COMO FUNCIONA:
 *   - cargarVector(): llena un vector de 30 enteros con 1 + rand() % 10.
 *   - calcularPromedios(): asigna p[i] = (v1[i] + v2[i] + v3[i]) / 3.0.
 *   - hallarMaximo(): recorre p[30] y retorna el mayor promedio encontrado.
 *
 * TIPOS DE DATOS UTILIZADOS:
 *   - int   : Vectores v1[30], v2[30], v3[30] e indices.
 *   - double: Vector p[30] de promedios reales y valor maximo.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

#define TAM 30

void cargarVector(int v[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        v[i] = 1 + rand() % 10;
    }
}

void calcularPromedios(const int v1[], const int v2[], const int v3[], double p[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        p[i] = (double)(v1[i] + v2[i] + v3[i]) / 3.0;
    }
}

double hallarMaximo(const double p[], int n, int *posMax) {
    int i;
    double max = p[0];
    *posMax = 0;
    for (i = 1; i < n; i++) {
        if (p[i] > max) {
            max = p[i];
            *posMax = i;
        }
    }
    return max;
}

void mostrarTabla(const int v1[], const int v2[], const int v3[], const double p[], int n) {
    int i;
    printf(" Pos |  v1  |  v2  |  v3  | Promedio p[i]\n");
    printf("-----+------+------+------+--------------\n");
    for (i = 0; i < n; i++) {
        printf(" %2d  |  %2d  |  %2d  |  %2d  |    %6.2f\n",
               i + 1, v1[i], v2[i], v3[i], p[i]);
    }
}

char leerSiNo(void) {
    int c;
    while (1) {
        c = _getch();
        if (c == 0 || c == 224) {
            _getch();
            continue;
        }
        if (c == 's' || c == 'S' || c == 'n' || c == 'N') {
            printf("%c\n", c);
            return (char)c;
        }
    }
}

int main(void) {
    int v1[TAM], v2[TAM], v3[TAM];
    double p[TAM], maxProm;
    int posMax;
    char repetir;

    srand((unsigned int)time(NULL));

    do {
        printf("\n==========================================================\n");
        printf("  FUNCIONES 13: 3 VECTORES DE 30, VECTOR PROMEDIO Y MAXIMO\n");
        printf("==========================================================\n\n");

        cargarVector(v1, TAM);
        cargarVector(v2, TAM);
        cargarVector(v3, TAM);

        calcularPromedios(v1, v2, v3, p, TAM);
        mostrarTabla(v1, v2, v3, p, TAM);

        maxProm = hallarMaximo(p, TAM, &posMax);
        printf("\n-> Valor maximo del vector p: %.2f (en posicion %d)\n", maxProm, posMax + 1);

        printf("\nDesea generar otros vectores? (S/N): ");
        repetir = leerSiNo();
    } while (repetir == 's' || repetir == 'S');

    printf("\nPresione cualquier tecla para finalizar...");
    _getch();
    return 0;
}
