/*
 * Ejercicio 40 (Matrices): Matriz 4x4 maximo, minimo y sus ubicaciones
 * -------------------------------------------------------------------
 * QUE HACE:
 *   Genera una matriz de 4x4 con numeros al azar del 0 al 9, la muestra
 *   en pantalla y halla el valor maximo y el minimo junto con sus
 *   ubicaciones (fila y columna).
 *
 * COMO FUNCIONA:
 *   - Guarda la primera posicion donde se encuentra el maximo y el minimo,
 *     y ademas lista todas las posiciones (fila, columna) en base 1 (y 0)
 *     donde aparecen dichos valores.
 *
 * TIPOS DE DATOS UTILIZADOS:
 *   - int : Matriz m[4][4], indices, maximo, minimo y coordenadas.
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
    int m[4][4];
    int i, j, maximo, minimo;
    int fMax, cMax, fMin, cMin;
    char repetir;

    srand((unsigned int)time(NULL));

    do {
        printf("\n==================================================\n");
        printf("  EJERCICIO 40: MATRIZ 4x4 MAX/MIN Y UBICACION\n");
        printf("==================================================\n\n");

        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++) {
                m[i][j] = rand() % 10;
            }
        }

        maximo = m[0][0];
        minimo = m[0][0];
        fMax = 0;
        cMax = 0;
        fMin = 0;
        cMin = 0;

        printf("Matriz generada (4x4):\n\n");
        printf("        Col 1 Col 2 Col 3 Col 4\n");
        for (i = 0; i < 4; i++) {
            printf("Fila %d |", i + 1);
            for (j = 0; j < 4; j++) {
                printf("  %2d  ", m[i][j]);
                if (m[i][j] > maximo) {
                    maximo = m[i][j];
                    fMax = i;
                    cMax = j;
                }
                if (m[i][j] < minimo) {
                    minimo = m[i][j];
                    fMin = i;
                    cMin = j;
                }
            }
            printf("|\n");
        }

        printf("\n-> Maximo: %d | Primera ubicacion: Fila %d, Columna %d (indices [%d][%d])\n",
               maximo, fMax + 1, cMax + 1, fMax, cMax);
        printf("   Todas las posiciones del maximo: ");
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++) {
                if (m[i][j] == maximo) {
                    printf("(F%d,C%d) ", i + 1, j + 1);
                }
            }
        }

        printf("\n\n-> Minimo: %d | Primera ubicacion: Fila %d, Columna %d (indices [%d][%d])\n",
               minimo, fMin + 1, cMin + 1, fMin, cMin);
        printf("   Todas las posiciones del minimo: ");
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++) {
                if (m[i][j] == minimo) {
                    printf("(F%d,C%d) ", i + 1, j + 1);
                }
            }
        }
        printf("\n");

        printf("\nDesea generar otra matriz? (S/N): ");
        repetir = leerSiNo();
    } while (repetir == 's' || repetir == 'S');

    printf("\nPresione cualquier tecla para finalizar...");
    _getch();
    return 0;
}
