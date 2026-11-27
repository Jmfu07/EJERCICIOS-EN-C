/*
 * Ejercicio 35 (Matrices): Matriz 3x5 de vocales al azar
 * -------------------------------------------------------------------
 * QUE HACE:
 *   Genera una matriz de 3 filas por 5 columnas rellena con vocales
 *   ('A', 'E', 'I', 'O', 'U') elegidas al azar y la muestra por pantalla.
 *
 * COMO FUNCIONA:
 *   - Define un arreglo constante de caracteres con las 5 vocales.
 *   - Con dos ciclos for anidados recorre la matriz 3x5 y selecciona
 *     un indice aleatorio entre 0 y 4 (rand() % 5).
 *   - Valida con _getch() si el usuario desea generar una nueva matriz.
 *
 * TIPOS DE DATOS UTILIZADOS:
 *   - char: Matriz m[3][5], arreglo de vocales y opcion de repeticion.
 *   - int : Indices de fila y columna (i, j).
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
    char m[3][5];
    int i, j;
    char repetir;

    srand((unsigned int)time(NULL));

    do {
        printf("\n==================================================\n");
        printf("    EJERCICIO 35: MATRIZ 3x5 CON VOCALES AL AZAR\n");
        printf("==================================================\n\n");

        for (i = 0; i < 3; i++) {
            for (j = 0; j < 5; j++) {
                m[i][j] = vocales[rand() % 5];
            }
        }

        printf("Matriz de vocales generada (3x5):\n\n");
        for (i = 0; i < 3; i++) {
            printf("   |");
            for (j = 0; j < 5; j++) {
                printf(" %c ", m[i][j]);
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
