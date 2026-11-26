/*
 * Ejercicio 43 (Matrices): Matriz m1 de 3x5 y promedios por fila y columna
 * -------------------------------------------------------------------
 * QUE HACE:
 *   Genera una matriz m1 de 3x5 con numeros al azar del 0 al 9, la muestra
 *   en pantalla y calcula los promedios de cada una de las 3 filas y de
 *   cada una de las 5 columnas.
 *
 * COMO FUNCIONA:
 *   - Suma cada fila i y divide entre 5.0 para hallar promFila[i].
 *   - Suma cada columna j y divide entre 3.0 para hallar promCol[j].
 *   - Muestra la tabla con los promedios al margen derecho e inferior.
 *
 * TIPOS DE DATOS UTILIZADOS:
 *   - int   : Matriz m1[3][5], indices y acumuladores parciales.
 *   - double: Arreglos promFila[3] y promCol[5].
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
    int m1[3][5];
    double promFila[3], promCol[5];
    int i, j, suma;
    char repetir;

    srand((unsigned int)time(NULL));

    do {
        printf("\n============================================================\n");
        printf("   EJERCICIO 43: MATRIZ 3x5 Y PROMEDIOS POR FILA Y COLUMNA\n");
        printf("============================================================\n\n");

        for (i = 0; i < 3; i++) {
            suma = 0;
            for (j = 0; j < 5; j++) {
                m1[i][j] = rand() % 10;
                suma += m1[i][j];
            }
            promFila[i] = (double)suma / 5.0;
        }

        for (j = 0; j < 5; j++) {
            suma = 0;
            for (i = 0; i < 3; i++) {
                suma += m1[i][j];
            }
            promCol[j] = (double)suma / 3.0;
        }

        printf("           Col 1  Col 2  Col 3  Col 4  Col 5  | Prom. Fila\n");
        printf("         -------------------------------------+-----------\n");
        for (i = 0; i < 3; i++) {
            printf("  Fila %d |", i + 1);
            for (j = 0; j < 5; j++) {
                printf("  %2d   ", m1[i][j]);
            }
            printf("|   %6.2f\n", promFila[i]);
        }
        printf("         -------------------------------------+-----------\n");
        printf("Prom.Col |");
        for (j = 0; j < 5; j++) {
            printf(" %5.2f ", promCol[j]);
        }
        printf("|\n");

        printf("\nDesea generar otra matriz? (S/N): ");
        repetir = leerSiNo();
    } while (repetir == 's' || repetir == 'S');

    printf("\nPresione cualquier tecla para finalizar...");
    _getch();
    return 0;
}
