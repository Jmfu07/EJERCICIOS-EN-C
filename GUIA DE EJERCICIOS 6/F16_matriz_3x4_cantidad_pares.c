/*
 * Ejercicio 16 (Funciones y procedimientos): Matriz 3x4 y cantidad de numeros pares
 * -------------------------------------------------------------------
 * QUE HACE:
 *   Carga una matriz de 3x4 con numeros al azar del 0 al 9, la muestra
 *   en pantalla y calcula la cantidad de numeros pares que contiene.
 *
 * COMO FUNCIONA:
 *   - cargarMatriz(): llena m[3][4] con rand() % 10.
 *   - mostrarMatriz(): imprime la matriz en formato tabular.
 *   - contarParesMatriz(): cuenta cuantos elementos cumplen m[i][j] % 2 == 0.
 *
 * TIPOS DE DATOS UTILIZADOS:
 *   - int : Matriz m[3][4], indices y contador de pares.
 *   - char: Opcion para repetir ejecucion.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

void cargarMatriz(int m[3][4]) {
    int i, j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 4; j++) {
            m[i][j] = rand() % 10;
        }
    }
}

void mostrarMatriz(int m[3][4]) {
    int i, j;
    printf("Matriz generada (3x4):\n\n");
    for (i = 0; i < 3; i++) {
        printf("   |");
        for (j = 0; j < 4; j++) {
            printf(" %2d ", m[i][j]);
        }
        printf("|\n");
    }
}

int contarParesMatriz(int m[3][4]) {
    int i, j, pares = 0;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 4; j++) {
            if (m[i][j] % 2 == 0) {
                pares++;
            }
        }
    }
    return pares;
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
    int m[3][4];
    int pares;
    char repetir;

    srand((unsigned int)time(NULL));

    do {
        printf("\n==========================================================\n");
        printf("   FUNCIONES 16: MATRIZ 3x4 Y CANTIDAD DE NUMEROS PARES\n");
        printf("==========================================================\n\n");

        cargarMatriz(m);
        mostrarMatriz(m);

        pares = contarParesMatriz(m);
        printf("\n-> Cantidad de numeros pares en la matriz: %d (de 12)\n", pares);

        printf("\nDesea generar otra matriz? (S/N): ");
        repetir = leerSiNo();
    } while (repetir == 's' || repetir == 'S');

    printf("\nPresione cualquier tecla para finalizar...");
    _getch();
    return 0;
}
