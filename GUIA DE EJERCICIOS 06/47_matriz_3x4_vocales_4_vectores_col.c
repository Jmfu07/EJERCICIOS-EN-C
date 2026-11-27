/*
 * Ejercicio 47 (Matrices): Matriz 3x4 de vocales a 4 vectores por columna
 * -------------------------------------------------------------------
 * QUE HACE:
 *   Genera una matriz de 3x4 con vocales al azar ('A','E','I','O','U'),
 *   la muestra en pantalla y crea 4 vectores de 3 elementos cada uno
 *   que contienen las vocales de la matriz por columna.
 *
 * COMO FUNCIONA:
 *   - Rellena m[3][4] con vocales aleatorias.
 *   - Copia cada columna j en un vector c1, c2, c3, c4 de tamano 3.
 *   - Muestra la matriz y los 4 vectores resultantes.
 *
 * TIPOS DE DATOS UTILIZADOS:
 *   - char: Matriz m[3][4] y vectores c1[3], c2[3], c3[3], c4[3].
 *   - int : Indices de fila y columna.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

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
    const char vocales[5] = {'A', 'E', 'I', 'O', 'U'};
    char m[3][4];
    char c1[3], c2[3], c3[3], c4[3];
    int i, j;
    char repetir;

    srand((unsigned int)time(NULL));

    do {
        printf("\n=======================================================\n");
        printf("  EJERCICIO 47: MATRIZ 3x4 VOCALES A 4 VECTORES (COL)\n");
        printf("=======================================================\n\n");

        for (i = 0; i < 3; i++) {
            for (j = 0; j < 4; j++) {
                m[i][j] = vocales[rand() % 5];
            }
        }

        for (i = 0; i < 3; i++) {
            c1[i] = m[i][0];
            c2[i] = m[i][1];
            c3[i] = m[i][2];
            c4[i] = m[i][3];
        }

        printf("Matriz de vocales (3x4):\n\n");
        printf("         C1  C2  C3  C4\n");
        for (i = 0; i < 3; i++) {
            printf("Fila %d |", i + 1);
            for (j = 0; j < 4; j++) {
                printf("  %c ", m[i][j]);
            }
            printf("|\n");
        }

        printf("\nVectores generados por columna (3 elementos cada uno):\n\n");
        printf("   Vector Columna 1: [ %c  %c  %c ]\n", c1[0], c1[1], c1[2]);
        printf("   Vector Columna 2: [ %c  %c  %c ]\n", c2[0], c2[1], c2[2]);
        printf("   Vector Columna 3: [ %c  %c  %c ]\n", c3[0], c3[1], c3[2]);
        printf("   Vector Columna 4: [ %c  %c  %c ]\n", c4[0], c4[1], c4[2]);

        printf("\nDesea generar otra matriz? (S/N): ");
        repetir = leerSiNo();
    } while (repetir == 's' || repetir == 'S');

    printf("\nPresione cualquier tecla para finalizar...");
    _getch();
    return 0;
}
