/*
 * Ejercicio 38 (Matrices): Matriz 4x4 al azar y promedio
 * -------------------------------------------------------------------
 * QUE HACE:
 *   Genera una matriz de 4x4 con numeros al azar del 0 al 9, la muestra
 *   en pantalla y calcula el promedio aritmetico de sus 16 elementos.
 *
 * COMO FUNCIONA:
 *   - Carga la matriz 4x4 con rand() % 10 y acumula la suma.
 *   - Divide la suma entre 16.0 para obtener el promedio real.
 *   - Permite repetir la ejecucion validando con _getch().
 *
 * TIPOS DE DATOS UTILIZADOS:
 *   - int   : Matriz m[4][4], indices (i, j) y suma acumulada.
 *   - double: Promedio de los elementos.
 *   - char  : Opcion de repeticion.
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
    int i, j, suma;
    double promedio;
    char repetir;

    srand((unsigned int)time(NULL));

    do {
        suma = 0;
        printf("\n==================================================\n");
        printf("     EJERCICIO 38: MATRIZ 4x4 AL AZAR Y PROMEDIO\n");
        printf("==================================================\n\n");

        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++) {
                m[i][j] = rand() % 10;
                suma += m[i][j];
            }
        }

        printf("Matriz generada (4x4):\n\n");
        for (i = 0; i < 4; i++) {
            printf("   |");
            for (j = 0; j < 4; j++) {
                printf(" %2d ", m[i][j]);
            }
            printf("|\n");
        }

        promedio = (double)suma / 16.0;
        printf("\n-> Sumatoria: %d\n", suma);
        printf("-> Promedio : %.2f\n", promedio);

        printf("\nDesea generar otra matriz? (S/N): ");
        repetir = leerSiNo();
    } while (repetir == 's' || repetir == 'S');

    printf("\nPresione cualquier tecla para finalizar...");
    _getch();
    return 0;
}
