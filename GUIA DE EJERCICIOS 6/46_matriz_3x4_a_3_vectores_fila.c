/*
 * Ejercicio 46 (Matrices): Matriz 3x4 a 3 vectores por fila
 * -------------------------------------------------------------------
 * QUE HACE:
 *   Genera una matriz de 3x4 con numeros al azar del 0 al 9, la muestra
 *   por pantalla y copia cada una de sus 3 filas en 3 vectores separados
 *   de 4 elementos cada uno.
 *
 * COMO FUNCIONA:
 *   - Asigna v1[j] = m[0][j], v2[j] = m[1][j] y v3[j] = m[2][j].
 *   - Muestra la matriz 3x4 y luego los 3 vectores resultantes.
 *
 * TIPOS DE DATOS UTILIZADOS:
 *   - int : Matriz m[3][4], vectores v1[4], v2[4], v3[4] e indices.
 *   - char: Opcion para repetir ejecucion.
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
    int m[3][4];
    int v1[4], v2[4], v3[4];
    int i, j;
    char repetir;

    srand((unsigned int)time(NULL));

    do {
        printf("\n==================================================\n");
        printf("  EJERCICIO 46: MATRIZ 3x4 A 3 VECTORES POR FILA\n");
        printf("==================================================\n\n");

        for (i = 0; i < 3; i++) {
            for (j = 0; j < 4; j++) {
                m[i][j] = rand() % 10;
            }
        }

        for (j = 0; j < 4; j++) {
            v1[j] = m[0][j];
            v2[j] = m[1][j];
            v3[j] = m[2][j];
        }

        printf("Matriz original (3x4):\n\n");
        for (i = 0; i < 3; i++) {
            printf("   Fila %d |", i + 1);
            for (j = 0; j < 4; j++) {
                printf(" %2d ", m[i][j]);
            }
            printf("|\n");
        }

        printf("\nVectores generados a partir de las filas:\n\n");
        printf("   Vector 1 (Fila 1): [");
        for (j = 0; j < 4; j++) printf(" %2d ", v1[j]);
        printf("]\n");

        printf("   Vector 2 (Fila 2): [");
        for (j = 0; j < 4; j++) printf(" %2d ", v2[j]);
        printf("]\n");

        printf("   Vector 3 (Fila 3): [");
        for (j = 0; j < 4; j++) printf(" %2d ", v3[j]);
        printf("]\n");

        printf("\nDesea generar otra matriz? (S/N): ");
        repetir = leerSiNo();
    } while (repetir == 's' || repetir == 'S');

    printf("\nPresione cualquier tecla para finalizar...");
    _getch();
    return 0;
}
