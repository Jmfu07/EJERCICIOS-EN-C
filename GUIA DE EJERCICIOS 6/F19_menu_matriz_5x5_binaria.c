/*
 * Ejercicio 19 (Funciones y procedimientos): Menu de matriz 5x5 binaria
 * -------------------------------------------------------------------
 * QUE HACE:
 *   Administra una matriz de 5x5 que inicialmente contiene todos ceros
 *   mediante un menu de opciones:
 *     1 - Mostrar la matriz
 *     2 - Mostrar la matriz traspuesta
 *     3 - Mostrar el inverso de la matriz (0 por 1 y viceversa)
 *     4 - Generar una nueva matriz (con numeros binarios aleatorios)
 *     5 - Salir
 *
 * COMO FUNCIONA:
 *   - inicializarEnCeros(): pone todas las celdas de m[5][5] en 0 al inicio.
 *   - Cada opcion del menu invoca a un procedimiento especifico.
 *   - La lectura de opcion usa _getch() aceptando solo teclas '1' a '5'.
 *
 * TIPOS DE DATOS UTILIZADOS:
 *   - int : Matriz m[5][5] e indices de fila y columna.
 *   - char: Opcion del menu ('1'..'5').
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

void inicializarEnCeros(int m[5][5]) {
    int i, j;
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            m[i][j] = 0;
        }
    }
}

void generarMatrizBinaria(int m[5][5]) {
    int i, j;
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            m[i][j] = rand() % 2;
        }
    }
}

void mostrarMatriz(int m[5][5]) {
    int i, j;
    printf("\nMatriz actual (5x5):\n\n");
    for (i = 0; i < 5; i++) {
        printf("   |");
        for (j = 0; j < 5; j++) {
            printf(" %2d ", m[i][j]);
        }
        printf("|\n");
    }
}

void mostrarTraspuesta(int m[5][5]) {
    int i, j;
    printf("\nMatriz Traspuesta (5x5):\n\n");
    for (i = 0; i < 5; i++) {
        printf("   |");
        for (j = 0; j < 5; j++) {
            printf(" %2d ", m[j][i]);
        }
        printf("|\n");
    }
}

void mostrarInverso(int m[5][5]) {
    int i, j;
    printf("\nInverso de la Matriz (0 <-> 1):\n\n");
    for (i = 0; i < 5; i++) {
        printf("   |");
        for (j = 0; j < 5; j++) {
            printf(" %2d ", (m[i][j] == 0) ? 1 : 0);
        }
        printf("|\n");
    }
}

char leerOpcion1a5(void) {
    int c;
    while (1) {
        c = _getch();
        if (c == 0 || c == 224) {
            _getch();
            continue;
        }
        if (c >= '1' && c <= '5') {
            printf("%c\n", c);
            return (char)c;
        }
    }
}

int main(void) {
    int m[5][5];
    char opcion;

    srand((unsigned int)time(NULL));
    inicializarEnCeros(m);

    printf("==========================================================\n");
    printf("      FUNCIONES 19: MENU DE MATRIZ 5x5 BINARIA\n");
    printf("==========================================================\n");
    printf("(La matriz 5x5 ha sido inicializada con ceros)\n");

    do {
        printf("\n------------------ MENU DE OPCIONES ------------------\n");
        printf("  1 - Mostrar la matriz\n");
        printf("  2 - Mostrar la matriz traspuesta\n");
        printf("  3 - Mostrar el inverso de la matriz (0 <-> 1)\n");
        printf("  4 - Generar una nueva matriz binaria aleatoria\n");
        printf("  5 - Salir\n");
        printf("------------------------------------------------------\n");
        printf("Seleccione una opcion (1-5): ");
        opcion = leerOpcion1a5();

        switch (opcion) {
            case '1':
                mostrarMatriz(m);
                break;
            case '2':
                mostrarTraspuesta(m);
                break;
            case '3':
                mostrarInverso(m);
                break;
            case '4':
                generarMatrizBinaria(m);
                printf("\n   [OK] Nueva matriz 5x5 binaria generada aleatoriamente.\n");
                mostrarMatriz(m);
                break;
            case '5':
                printf("\nSaliendo del programa...\n");
                break;
        }
    } while (opcion != '5');

    printf("\nPresione cualquier tecla para finalizar...");
    _getch();
    return 0;
}
