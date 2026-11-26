/*
 * Ejercicio 17 (Funciones y procedimientos): Matriz 4x4 a 4 vectores por fila
 * -------------------------------------------------------------------
 * QUE HACE:
 *   Carga una matriz de 4x4 con numeros al azar del 0 al 9, la muestra
 *   en pantalla y crea 4 vectores que contienen las filas de la matriz,
 *   mostrandolos por pantalla.
 *
 * COMO FUNCIONA:
 *   - cargarMatriz(): llena m[4][4] con rand() % 10.
 *   - extraerFila(): copia la fila indicada de la matriz en un vector de 4
 *     posiciones.
 *   - mostrarVector(): imprime cada uno de los 4 vectores.
 *
 * TIPOS DE DATOS UTILIZADOS:
 *   - int : Matriz m[4][4], vectores v1[4], v2[4], v3[4], v4[4] e indices.
 *   - char: Opcion para repetir ejecucion.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

void cargarMatriz(int m[4][4]) {
    int i, j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            m[i][j] = rand() % 10;
        }
    }
}

void mostrarMatriz(int m[4][4]) {
    int i, j;
    printf("Matriz generada (4x4):\n\n");
    for (i = 0; i < 4; i++) {
        printf("   Fila %d |", i + 1);
        for (j = 0; j < 4; j++) {
            printf(" %2d ", m[i][j]);
        }
        printf("|\n");
    }
}

void extraerFila(int m[4][4], int fila, int v[4]) {
    int j;
    for (j = 0; j < 4; j++) {
        v[j] = m[fila][j];
    }
}

void mostrarVector(const char *nombre, const int v[4]) {
    int j;
    printf("   %s: [", nombre);
    for (j = 0; j < 4; j++) {
        printf(" %2d ", v[j]);
    }
    printf("]\n");
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
    int v1[4], v2[4], v3[4], v4[4];
    char repetir;

    srand((unsigned int)time(NULL));

    do {
        printf("\n==========================================================\n");
        printf("   FUNCIONES 17: MATRIZ 4x4 A 4 VECTORES CON SUS FILAS\n");
        printf("==========================================================\n\n");

        cargarMatriz(m);
        mostrarMatriz(m);

        extraerFila(m, 0, v1);
        extraerFila(m, 1, v2);
        extraerFila(m, 2, v3);
        extraerFila(m, 3, v4);

        printf("\nVectores extraidos de las 4 filas:\n\n");
        mostrarVector("Vector 1 (Fila 1)", v1);
        mostrarVector("Vector 2 (Fila 2)", v2);
        mostrarVector("Vector 3 (Fila 3)", v3);
        mostrarVector("Vector 4 (Fila 4)", v4);

        printf("\nDesea generar otra matriz? (S/N): ");
        repetir = leerSiNo();
    } while (repetir == 's' || repetir == 'S');

    printf("\nPresione cualquier tecla para finalizar...");
    _getch();
    return 0;
}
