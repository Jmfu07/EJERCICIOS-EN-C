/*
 * Ejercicio 7 (Funciones y procedimientos): Vector 20 enteros, promedio, pares e impares
 * -------------------------------------------------------------------
 * QUE HACE:
 *   Genera un vector con 20 numeros enteros al azar del 1 al 10, lo muestra
 *   en pantalla y calcula el promedio, la cantidad de numeros pares y la
 *   cantidad de numeros impares.
 *
 * COMO FUNCIONA:
 *   - cargarVector(): llena el vector con valores aleatorios [1, 10].
 *   - calcularPromedio(): suma los 20 elementos y divide por 20.0.
 *   - contarPares() y contarImpares(): determinan la cantidad de cada tipo.
 *
 * TIPOS DE DATOS UTILIZADOS:
 *   - int   : Vector v[20], indices y contadores de pares e impares.
 *   - double: Promedio general del vector.
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

void mostrarVector(const int v[], int n) {
    int i;
    printf("Vector generado (%d elementos):\n   [", n);
    for (i = 0; i < n; i++) {
        printf("%d%s", v[i], (i < n - 1) ? ", " : "");
    }
    printf("]\n");
}

double calcularPromedio(const int v[], int n) {
    int i, suma = 0;
    for (i = 0; i < n; i++) {
        suma += v[i];
    }
    return (double)suma / (double)n;
}

int contarPares(const int v[], int n) {
    int i, cant = 0;
    for (i = 0; i < n; i++) {
        if (v[i] % 2 == 0) {
            cant++;
        }
    }
    return cant;
}

int contarImpares(const int v[], int n) {
    int i, cant = 0;
    for (i = 0; i < n; i++) {
        if (v[i] % 2 != 0) {
            cant++;
        }
    }
    return cant;
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
    int v[TAM];
    char repetir;

    srand((unsigned int)time(NULL));

    do {
        printf("\n==========================================================\n");
        printf("  FUNCIONES 07: VECTOR 20 (PROMEDIO, PARES E IMPARES)\n");
        printf("==========================================================\n\n");

        cargarVector(v, TAM);
        mostrarVector(v, TAM);

        printf("\n-> Promedio de los elementos : %.2f\n", calcularPromedio(v, TAM));
        printf("-> Cantidad de numeros pares : %d\n", contarPares(v, TAM));
        printf("-> Cantidad de numeros impares: %d\n", contarImpares(v, TAM));

        printf("\nDesea generar otro vector? (S/N): ");
        repetir = leerSiNo();
    } while (repetir == 's' || repetir == 'S');

    printf("\nPresione cualquier tecla para finalizar...");
    _getch();
    return 0;
}
