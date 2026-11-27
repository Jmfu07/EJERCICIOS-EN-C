/*
 * Ejercicio 44 (Matrices): Matriz m1 de 10x6 y vector v1 con promedios por fila
 * -------------------------------------------------------------------
 * QUE HACE:
 *   Genera una matriz m1 de 10x6 con numeros al azar del 0 al 9, la muestra
 *   en pantalla y guarda los promedios de cada fila en un vector v1 de
 *   10 elementos.
 *
 * COMO FUNCIONA:
 *   - Para cada fila i (0..9), genera 6 valores aleatorios (0..9) y los
 *     suma.
 *   - Asigna v1[i] = suma / 6.0.
 *   - Muestra la matriz m1 y a continuacion el vector v1.
 *
 * TIPOS DE DATOS UTILIZADOS:
 *   - int   : Matriz m1[10][6], indices (i, j) y acumulador suma.
 *   - double: Vector v1[10] de promedios por fila.
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
    int m1[10][6];
    double v1[10];
    int i, j, suma;
    char repetir;

    srand((unsigned int)time(NULL));

    do {
        printf("\n============================================================\n");
        printf("  EJERCICIO 44: MATRIZ 10x6 Y VECTOR v1 DE PROMEDIOS/FILA\n");
        printf("============================================================\n\n");

        for (i = 0; i < 10; i++) {
            suma = 0;
            for (j = 0; j < 6; j++) {
                m1[i][j] = rand() % 10;
                suma += m1[i][j];
            }
            v1[i] = (double)suma / 6.0;
        }

        printf("Matriz m1 (10x6)                     | Vector v1 (Promedio)\n");
        printf("-------------------------------------+---------------------\n");
        for (i = 0; i < 10; i++) {
            printf("Fila %2d: |", i + 1);
            for (j = 0; j < 6; j++) {
                printf(" %2d ", m1[i][j]);
            }
            printf("|  ->  v1[%2d] = %6.2f\n", i, v1[i]);
        }

        printf("\nDesea generar otra matriz? (S/N): ");
        repetir = leerSiNo();
    } while (repetir == 's' || repetir == 'S');

    printf("\nPresione cualquier tecla para finalizar...");
    _getch();
    return 0;
}
