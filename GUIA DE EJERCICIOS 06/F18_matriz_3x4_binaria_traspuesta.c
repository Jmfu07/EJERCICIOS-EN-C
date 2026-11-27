/*
 * Ejercicio 18 (Funciones y procedimientos): Matriz 3x4 binaria y su traspuesta 4x3
 * -------------------------------------------------------------------
 * QUE HACE:
 *   Genera una matriz de 3x4 con numeros binarios al azar (0 y 1),
 *   la muestra en pantalla y crea otra matriz de 4x3 con su traspuesta,
 *   mostrandola tambien.
 *
 * COMO FUNCIONA:
 *   - cargarMatrizBinaria(): llena m[3][4] con rand() % 2.
 *   - trasponerMatriz(): copia m[i][j] en t[j][i] para i=0..2, j=0..3.
 *   - mostrarMatriz3x4() y mostrarMatriz4x3(): muestran ambas matrices.
 *
 * TIPOS DE DATOS UTILIZADOS:
 *   - int : Matriz m[3][4], matriz traspuesta t[4][3] e indices.
 *   - char: Opcion para repetir ejecucion.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

void cargarMatrizBinaria(int m[3][4]) {
    int i, j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 4; j++) {
            m[i][j] = rand() % 2;
        }
    }
}

void trasponerMatriz(int m[3][4], int t[4][3]) {
    int i, j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 4; j++) {
            t[j][i] = m[i][j];
        }
    }
}

void mostrarMatriz3x4(int m[3][4]) {
    int i, j;
    printf("Matriz Binaria Original (3x4):\n\n");
    for (i = 0; i < 3; i++) {
        printf("   |");
        for (j = 0; j < 4; j++) {
            printf(" %2d ", m[i][j]);
        }
        printf("|\n");
    }
}

void mostrarMatriz4x3(int t[4][3]) {
    int i, j;
    printf("\nMatriz Traspuesta (4x3):\n\n");
    for (i = 0; i < 4; i++) {
        printf("   |");
        for (j = 0; j < 3; j++) {
            printf(" %2d ", t[i][j]);
        }
        printf("|\n");
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
    int m[3][4], t[4][3];
    char repetir;

    srand((unsigned int)time(NULL));

    do {
        printf("\n==========================================================\n");
        printf("   FUNCIONES 18: MATRIZ 3x4 BINARIA Y TRASPUESTA (4x3)\n");
        printf("==========================================================\n\n");

        cargarMatrizBinaria(m);
        trasponerMatriz(m, t);

        mostrarMatriz3x4(m);
        mostrarMatriz4x3(t);

        printf("\nDesea generar otra matriz? (S/N): ");
        repetir = leerSiNo();
    } while (repetir == 's' || repetir == 'S');

    printf("\nPresione cualquier tecla para finalizar...");
    _getch();
    return 0;
}
