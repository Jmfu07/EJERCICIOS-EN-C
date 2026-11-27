/*
 * Ejercicio 42 (Matrices): Suma de dos matrices de 4x4
 * -------------------------------------------------------------------
 * QUE HACE:
 *   Genera dos matrices de 4x4 con numeros al azar del 0 al 9, las
 *   muestra en pantalla y calcula la suma elemento a elemento en una
 *   tercera matriz de 4x4.
 *
 * COMO FUNCIONA:
 *   - Rellena las matrices A y B con rand() % 10.
 *   - Calcula C[i][j] = A[i][j] + B[i][j] para cada posicion.
 *   - Imprime las tres matrices alineadas.
 *
 * TIPOS DE DATOS UTILIZADOS:
 *   - int : Matrices A[4][4], B[4][4], C[4][4] e indices (i, j).
 *   - char: Opcion de repeticion validada con _getch().
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
    int a[4][4], b[4][4], suma[4][4];
    int i, j;
    char repetir;

    srand((unsigned int)time(NULL));

    do {
        printf("\n==================================================\n");
        printf("     EJERCICIO 42: SUMA DE DOS MATRICES 4x4\n");
        printf("==================================================\n\n");

        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++) {
                a[i][j] = rand() % 10;
                b[i][j] = rand() % 10;
                suma[i][j] = a[i][j] + b[i][j];
            }
        }

        printf("      MATRIZ A                  MATRIZ B                MATRIZ SUMA (A+B)\n");
        for (i = 0; i < 4; i++) {
            printf(" |");
            for (j = 0; j < 4; j++) {
                printf(" %2d ", a[i][j]);
            }
            printf("|   +   |");
            for (j = 0; j < 4; j++) {
                printf(" %2d ", b[i][j]);
            }
            printf("|   =   |");
            for (j = 0; j < 4; j++) {
                printf(" %2d ", suma[i][j]);
            }
            printf("|\n");
        }

        printf("\nDesea generar otras matrices? (S/N): ");
        repetir = leerSiNo();
    } while (repetir == 's' || repetir == 'S');

    printf("\nPresione cualquier tecla para finalizar...");
    _getch();
    return 0;
}
