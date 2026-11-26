/*
 * Ejercicio 15 (Funciones y procedimientos): Matriz 4x4 con dado y suma de columnas pares
 * -------------------------------------------------------------------
 * QUE HACE:
 *   Llena una matriz de 4x4 con numeros de un dado al azar (1 al 6),
 *   la muestra por pantalla y halla la sumatoria de los numeros que se
 *   encuentran en las columnas pares.
 *
 * COMO FUNCIONA:
 *   - cargarMatrizDado(): llena la matriz 4x4 con 1 + rand() % 6.
 *   - mostrarMatriz(): imprime la matriz con encabezados de columna.
 *   - sumarColumnasParesBase1() y sumarColumnasParesIndiceC(): calculan
 *     la suma en las columnas pares (Columnas 2 y 4, e indices 0 y 2).
 *
 * TIPOS DE DATOS UTILIZADOS:
 *   - int : Matriz m[4][4], indices y acumuladores.
 *   - char: Opcion para repetir ejecucion.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

void cargarMatrizDado(int m[4][4]) {
    int i, j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            m[i][j] = 1 + rand() % 6;
        }
    }
}

void mostrarMatriz(int m[4][4]) {
    int i, j;
    printf("Matriz 4x4 (valores de dado 1..6):\n\n");
    printf("           C1  C2  C3  C4\n");
    for (i = 0; i < 4; i++) {
        printf("   Fila %d |", i + 1);
        for (j = 0; j < 4; j++) {
            printf(" %2d ", m[i][j]);
        }
        printf("|\n");
    }
}

int sumarColumnasParesBase1(int m[4][4]) {
    int i, j, suma = 0;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if ((j + 1) % 2 == 0) {
                suma += m[i][j];
            }
        }
    }
    return suma;
}

int sumarColumnasParesIndiceC(int m[4][4]) {
    int i, j, suma = 0;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if (j % 2 == 0) {
                suma += m[i][j];
            }
        }
    }
    return suma;
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
    int m[4][4];
    char repetir;

    srand((unsigned int)time(NULL));

    do {
        printf("\n==========================================================\n");
        printf("  FUNCIONES 15: MATRIZ 4x4 DADO Y SUMA DE COLUMNAS PARES\n");
        printf("==========================================================\n\n");

        cargarMatrizDado(m);
        mostrarMatriz(m);

        printf("\n-> Sumatoria en columnas pares (Columnas 2 y 4): %d\n", sumarColumnasParesBase1(m));
        printf("-> Sumatoria por indice C par  (Indices 0 y 2) : %d\n", sumarColumnasParesIndiceC(m));

        printf("\nDesea generar otra matriz? (S/N): ");
        repetir = leerSiNo();
    } while (repetir == 's' || repetir == 'S');

    printf("\nPresione cualquier tecla para finalizar...");
    _getch();
    return 0;
}
