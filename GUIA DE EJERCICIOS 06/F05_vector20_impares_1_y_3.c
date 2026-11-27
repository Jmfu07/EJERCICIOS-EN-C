/*
 * Ejercicio 5 (Funciones y procedimientos): Vector 20 enteros, impares y cantidad de 1 y 3
 * -------------------------------------------------------------------
 * QUE HACE:
 *   Genera un vector con 20 numeros enteros al azar del 1 al 10, lo muestra,
 *   calcula la cantidad total de numeros impares y la cantidad de numeros
 *   1 y 3 utilizando funciones y procedimientos.
 *
 * COMO FUNCIONA:
 *   - cargarVector(): genera 20 enteros entre 1 y 10.
 *   - contarImpares(): cuenta cuantos elementos cumplen v[i] % 2 != 0.
 *   - contarValor(): cuenta cuantas veces aparece un valor especifico (1 o 3).
 *
 * TIPOS DE DATOS UTILIZADOS:
 *   - int : Vector v[20], indices y contadores.
 *   - char: Opcion para repetir ejecucion.
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

int contarImpares(const int v[], int n) {
    int i, cant = 0;
    for (i = 0; i < n; i++) {
        if (v[i] % 2 != 0) {
            cant++;
        }
    }
    return cant;
}

int contarValor(const int v[], int n, int buscado) {
    int i, cant = 0;
    for (i = 0; i < n; i++) {
        if (v[i] == buscado) {
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
    int impares, cant1, cant3;
    char repetir;

    srand((unsigned int)time(NULL));

    do {
        printf("\n==========================================================\n");
        printf("  FUNCIONES 05: VECTOR 20 (IMPARES Y CANTIDAD DE 1 Y 3)\n");
        printf("==========================================================\n\n");

        cargarVector(v, TAM);
        mostrarVector(v, TAM);

        impares = contarImpares(v, TAM);
        cant1 = contarValor(v, TAM, 1);
        cant3 = contarValor(v, TAM, 3);

        printf("\n-> Cantidad total de numeros impares : %d\n", impares);
        printf("-> Cantidad de numeros 1             : %d\n", cant1);
        printf("-> Cantidad de numeros 3             : %d\n", cant3);
        printf("-> Cantidad conjunta de numeros 1 y 3: %d\n", cant1 + cant3);

        printf("\nDesea generar otro vector? (S/N): ");
        repetir = leerSiNo();
    } while (repetir == 's' || repetir == 'S');

    printf("\nPresione cualquier tecla para finalizar...");
    _getch();
    return 0;
}
