/*
 * Ejercicio 39 (Matrices): Matriz 4x4 al azar, maximo y minimo
 * -------------------------------------------------------------------
 * QUE HACE:
 *   Genera una matriz de 4x4 con numeros al azar del 0 al 9, la muestra
 *   en pantalla y determina el valor maximo y el valor minimo.
 *
 * COMO FUNCIONA:
 *   - Inicializa 'maximo' y 'minimo' con el primer elemento m[0][0].
 *   - Recorre toda la matriz comparando cada elemento con 'maximo' y 'minimo'.
 *   - Permite repetir la prueba validando con _getch().
 *
 * TIPOS DE DATOS UTILIZADOS:
 *   - int : Matriz m[4][4], indices (i, j), maximo y minimo.
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
    int m[4][4];
    int i, j, maximo, minimo;
    char repetir;

    srand((unsigned int)time(NULL));

    do {
        printf("\n==================================================\n");
        printf("    EJERCICIO 39: MATRIZ 4x4 MAXIMO Y MINIMO\n");
        printf("==================================================\n\n");

        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++) {
                m[i][j] = rand() % 10;
            }
        }

        maximo = m[0][0];
        minimo = m[0][0];

        printf("Matriz generada (4x4):\n\n");
        for (i = 0; i < 4; i++) {
            printf("   |");
            for (j = 0; j < 4; j++) {
                printf(" %2d ", m[i][j]);
                if (m[i][j] > maximo) {
                    maximo = m[i][j];
                }
                if (m[i][j] < minimo) {
                    minimo = m[i][j];
                }
            }
            printf("|\n");
        }

        printf("\n-> Valor Maximo: %d\n", maximo);
        printf("-> Valor Minimo: %d\n", minimo);

        printf("\nDesea generar otra matriz? (S/N): ");
        repetir = leerSiNo();
    } while (repetir == 's' || repetir == 'S');

    printf("\nPresione cualquier tecla para finalizar...");
    _getch();
    return 0;
}
