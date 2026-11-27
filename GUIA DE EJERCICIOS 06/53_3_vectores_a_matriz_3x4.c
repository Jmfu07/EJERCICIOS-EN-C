/*
 * Ejercicio 53 (Matrices): 3 vectores de 4 elementos a una matriz de 3x4
 * -------------------------------------------------------------------
 * QUE HACE:
 *   Genera 3 vectores de 4 elementos cada uno con numeros al azar del
 *   1 al 9, y construye una matriz de 3x4 que contiene los elementos
 *   de dichos vectores.
 *
 * COMO FUNCIONA:
 *   - Carga v1[4], v2[4] y v3[4] con 1 + rand() % 9.
 *   - Copia v1 en la fila 0, v2 en la fila 1 y v3 en la fila 2 de m[3][4].
 *   - Muestra los 3 vectores y la matriz 3x4 resultante.
 *
 * TIPOS DE DATOS UTILIZADOS:
 *   - int : Vectores v1[4], v2[4], v3[4], matriz m[3][4] e indices.
 *   - char: Opcion de repeticion.
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
    int v1[4], v2[4], v3[4];
    int m[3][4];
    int i, j;
    char repetir;

    srand((unsigned int)time(NULL));

    do {
        printf("\n=======================================================\n");
        printf("  EJERCICIO 53: 3 VECTORES (TAM 4) A MATRIZ DE 3x4\n");
        printf("=======================================================\n\n");

        for (j = 0; j < 4; j++) {
            v1[j] = 1 + rand() % 9;
            v2[j] = 1 + rand() % 9;
            v3[j] = 1 + rand() % 9;

            m[0][j] = v1[j];
            m[1][j] = v2[j];
            m[2][j] = v3[j];
        }

        printf("Vectores generados (azar 1 al 9):\n\n");
        printf("   Vector 1: [");
        for (j = 0; j < 4; j++) printf(" %2d ", v1[j]);
        printf("]\n");

        printf("   Vector 2: [");
        for (j = 0; j < 4; j++) printf(" %2d ", v2[j]);
        printf("]\n");

        printf("   Vector 3: [");
        for (j = 0; j < 4; j++) printf(" %2d ", v3[j]);
        printf("]\n");

        printf("\nMatriz resultante (3x4):\n\n");
        for (i = 0; i < 3; i++) {
            printf("   Fila %d |", i + 1);
            for (j = 0; j < 4; j++) {
                printf(" %2d ", m[i][j]);
            }
            printf("|\n");
        }

        printf("\nDesea repetir la generacion? (S/N): ");
        repetir = leerSiNo();
    } while (repetir == 's' || repetir == 'S');

    printf("\nPresione cualquier tecla para finalizar...");
    _getch();
    return 0;
}
