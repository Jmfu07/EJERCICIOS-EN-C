/*
 * Ejercicio 14 (Funciones y procedimientos): Matriz 4x4 con dado y suma de filas impares
 * -------------------------------------------------------------------
 * QUE HACE:
 *   Llena una matriz de 4x4 con numeros de un dado al azar (1 al 6),
 *   la muestra por pantalla y halla la sumatoria de los numeros que se
 *   encuentran en las filas impares.
 *
 * COMO FUNCIONA:
 *   - cargarMatrizDado(): llena la matriz 4x4 con 1 + rand() % 6.
 *   - mostrarMatriz(): imprime las 4 filas indicando su numero.
 *   - sumarFilasImpares(): calcula la suma de las filas impares (Filas 1 y 3)
 *     y tambien muestra la suma por indice C impar (indices 1 y 3).
 *
 * TIPOS DE DATOS UTILIZADOS:
 *   - int : Matriz m[4][4], indices y acumuladores de suma.
 *   - char: Opcion de repeticion.
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
    for (i = 0; i < 4; i++) {
        printf("   Fila %d (ind %d) |", i + 1, i);
        for (j = 0; j < 4; j++) {
            printf(" %2d ", m[i][j]);
        }
        printf("|%s\n", ((i + 1) % 2 != 0) ? " <-- Fila impar" : "");
    }
}

int sumarFilasImparesBase1(int m[4][4]) {
    int i, j, suma = 0;
    for (i = 0; i < 4; i++) {
        if ((i + 1) % 2 != 0) {
            for (j = 0; j < 4; j++) {
                suma += m[i][j];
            }
        }
    }
    return suma;
}

int sumarFilasImparesIndiceC(int m[4][4]) {
    int i, j, suma = 0;
    for (i = 0; i < 4; i++) {
        if (i % 2 != 0) {
            for (j = 0; j < 4; j++) {
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
        printf("  FUNCIONES 14: MATRIZ 4x4 DADO Y SUMA DE FILAS IMPARES\n");
        printf("==========================================================\n\n");

        cargarMatrizDado(m);
        mostrarMatriz(m);

        printf("\n-> Sumatoria en filas impares (Filas 1 y 3)  : %d\n", sumarFilasImparesBase1(m));
        printf("-> Sumatoria por indice C impar (Indices 1,3): %d\n", sumarFilasImparesIndiceC(m));

        printf("\nDesea generar otra matriz? (S/N): ");
        repetir = leerSiNo();
    } while (repetir == 's' || repetir == 'S');

    printf("\nPresione cualquier tecla para finalizar...");
    _getch();
    return 0;
}
