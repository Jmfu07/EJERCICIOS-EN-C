/*
 * Ejercicio 34 (Matrices): Matriz 3x3 al azar
 * -------------------------------------------------------------------
 * QUE HACE:
 *   Genera una matriz de 3 filas por 3 columnas con numeros enteros
 *   aleatorios entre 0 y 9, y la muestra en pantalla en formato de tabla.
 *
 * COMO FUNCIONA:
 *   - Inicializa la semilla aleatoria con srand(time(NULL)).
 *   - Recorre las 3 filas y 3 columnas con dos ciclos for anidados,
 *     asignando rand() % 10 a cada celda.
 *   - Permite repetir la generacion validando la opcion con _getch().
 *
 * TIPOS DE DATOS UTILIZADOS:
 *   - int: Para la matriz m[3][3], indices de recorrido (i, j) y tecla.
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
    int m[3][3];
    int i, j;
    char repetir;

    srand((unsigned int)time(NULL));

    do {
        printf("\n==================================================\n");
        printf("      EJERCICIO 34: MATRIZ 3x3 AL AZAR (0-9)\n");
        printf("==================================================\n\n");

        for (i = 0; i < 3; i++) {
            for (j = 0; j < 3; j++) {
                m[i][j] = rand() % 10;
            }
        }

        printf("Matriz generada (3x3):\n\n");
        for (i = 0; i < 3; i++) {
            printf("   |");
            for (j = 0; j < 3; j++) {
                printf(" %2d ", m[i][j]);
            }
            printf("|\n");
        }

        printf("\nDesea generar otra matriz? (S/N): ");
        repetir = leerSiNo();
    } while (repetir == 's' || repetir == 'S');

    printf("\nPresione cualquier tecla para finalizar...");
    _getch();
    return 0;
}
