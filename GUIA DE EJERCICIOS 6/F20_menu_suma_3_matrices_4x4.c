/*
 * Ejercicio 20 (Funciones y procedimientos): Menu para sumar 3 matrices de 4x4
 * -------------------------------------------------------------------
 * QUE HACE:
 *   Genera 3 matrices (m1, m2 y m3) de 4x4 con numeros al azar del 0 al 9
 *   y mediante un menu de opciones permite hallar y mostrar la suma de:
 *     1 - m1 + m2
 *     2 - m2 + m3
 *     3 - m1 + m3
 *     4 - Mostrar nuevamente m1, m2 y m3
 *     5 - Generar nuevas matrices m1, m2 y m3
 *     6 - Salir
 *
 * COMO FUNCIONA:
 *   - cargarMatriz(): llena una matriz 4x4 con rand() % 10.
 *   - sumarYMostrar(): suma dos matrices 4x4 recibidas por parametro y
 *     muestra las dos matrices operando junto con la matriz resultado.
 *
 * TIPOS DE DATOS UTILIZADOS:
 *   - int : Matrices m1[4][4], m2[4][4], m3[4][4] e indices.
 *   - char: Opcion del menu ('1'..'6').
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

void mostrarTresMatrices(int m1[4][4], int m2[4][4], int m3[4][4]) {
    int i, j;
    printf("\n        MATRIZ m1                 MATRIZ m2                 MATRIZ m3\n");
    for (i = 0; i < 4; i++) {
        printf(" |");
        for (j = 0; j < 4; j++) printf(" %2d ", m1[i][j]);
        printf("|     |");
        for (j = 0; j < 4; j++) printf(" %2d ", m2[i][j]);
        printf("|     |");
        for (j = 0; j < 4; j++) printf(" %2d ", m3[i][j]);
        printf("|\n");
    }
}

void sumarYMostrar(const char *etiqueta, int a[4][4], int b[4][4]) {
    int r[4][4];
    int i, j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            r[i][j] = a[i][j] + b[i][j];
        }
    }
    printf("\nResultado de %s:\n\n", etiqueta);
    for (i = 0; i < 4; i++) {
        printf(" |");
        for (j = 0; j < 4; j++) printf(" %2d ", a[i][j]);
        printf("|  +  |");
        for (j = 0; j < 4; j++) printf(" %2d ", b[i][j]);
        printf("|  =  |");
        for (j = 0; j < 4; j++) printf(" %2d ", r[i][j]);
        printf("|\n");
    }
}

char leerOpcion1a6(void) {
    int c;
    while (1) {
        c = _getch();
        if (c == 0 || c == 224) {
            _getch();
            continue;
        }
        if (c >= '1' && c <= '6') {
            printf("%c\n", c);
            return (char)c;
        }
    }
}

int main(void) {
    int m1[4][4], m2[4][4], m3[4][4];
    char opcion;

    srand((unsigned int)time(NULL));

    cargarMatriz(m1);
    cargarMatriz(m2);
    cargarMatriz(m3);

    printf("==========================================================\n");
    printf("   FUNCIONES 20: MENU DE SUMA DE 3 MATRICES 4x4\n");
    printf("==========================================================\n");

    mostrarTresMatrices(m1, m2, m3);

    do {
        printf("\n------------------ MENU DE OPCIONES ------------------\n");
        printf("  1 - Sumar m1 + m2\n");
        printf("  2 - Sumar m2 + m3\n");
        printf("  3 - Sumar m1 + m3\n");
        printf("  4 - Mostrar m1, m2 y m3\n");
        printf("  5 - Generar nuevas matrices m1, m2 y m3\n");
        printf("  6 - Salir\n");
        printf("------------------------------------------------------\n");
        printf("Seleccione una opcion (1-6): ");
        opcion = leerOpcion1a6();

        switch (opcion) {
            case '1':
                sumarYMostrar("m1 + m2", m1, m2);
                break;
            case '2':
                sumarYMostrar("m2 + m3", m2, m3);
                break;
            case '3':
                sumarYMostrar("m1 + m3", m1, m3);
                break;
            case '4':
                mostrarTresMatrices(m1, m2, m3);
                break;
            case '5':
                cargarMatriz(m1);
                cargarMatriz(m2);
                cargarMatriz(m3);
                mostrarTresMatrices(m1, m2, m3);
                break;
            case '6':
                printf("\nSaliendo del programa...\n");
                break;
        }
    } while (opcion != '6');

    printf("\nPresione cualquier tecla para finalizar...");
    _getch();
    return 0;
}
