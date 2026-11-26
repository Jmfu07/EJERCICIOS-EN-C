/*
 * Ejercicio 11 (Funciones y procedimientos): Dos vectores de 20, suma y promedio
 * -------------------------------------------------------------------
 * QUE HACE:
 *   Genera dos vectores de 20 numeros enteros al azar del 1 al 10,
 *   suma cada elemento de ambos vectores en un tercer vector, calcula el
 *   promedio de cada posicion en un cuarto vector y finalmente muestra
 *   los cuatro vectores resultantes.
 *
 * COMO FUNCIONA:
 *   - cargarVector(): llena v1 y v2 con enteros aleatorios [1, 10].
 *   - calcularSumaYPromedio(): para cada posicion i (0..19), calcula
 *     vSuma[i] = v1[i] + v2[i] y vProm[i] = (v1[i] + v2[i]) / 2.0.
 *
 * TIPOS DE DATOS UTILIZADOS:
 *   - int   : Vectores v1[20], v2[20], vSuma[20] e indices.
 *   - double: Vector vProm[20] con los promedios por posicion.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

#define TAM 20

void cargarVector(int v[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        v[i] = 1 + rand() % 10;
    }
}

void calcularSumaYPromedio(const int v1[], const int v2[], int vSuma[], double vProm[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        vSuma[i] = v1[i] + v2[i];
        vProm[i] = (double)vSuma[i] / 2.0;
    }
}

void mostrarCuatroVectores(const int v1[], const int v2[], const int vSuma[], const double vProm[], int n) {
    int i;
    printf(" Pos | Vector 1 | Vector 2 | Vector Suma | Vector Promedio\n");
    printf("-----+----------+----------+-------------+----------------\n");
    for (i = 0; i < n; i++) {
        printf(" %2d  |    %2d    |    %2d    |     %2d      |      %5.2f\n",
               i + 1, v1[i], v2[i], vSuma[i], vProm[i]);
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
    int v1[TAM], v2[TAM], vSuma[TAM];
    double vProm[TAM];
    char repetir;

    srand((unsigned int)time(NULL));

    do {
        printf("\n==========================================================\n");
        printf("  FUNCIONES 11: DOS VECTORES DE 20, SUMA Y PROMEDIO\n");
        printf("==========================================================\n\n");

        cargarVector(v1, TAM);
        cargarVector(v2, TAM);
        calcularSumaYPromedio(v1, v2, vSuma, vProm, TAM);
        mostrarCuatroVectores(v1, v2, vSuma, vProm, TAM);

        printf("\nDesea generar nuevos vectores? (S/N): ");
        repetir = leerSiNo();
    } while (repetir == 's' || repetir == 'S');

    printf("\nPresione cualquier tecla para finalizar...");
    _getch();
    return 0;
}
