/*
 * Ejercicio 54 (Matrices): 4 vectores de tamano 5 con vocales a matriz 5x4
 * -------------------------------------------------------------------
 * QUE HACE:
 *   Genera 4 vectores de tamano 5 con vocales al azar ('A','E','I','O','U')
 *   y construye una matriz de 5 filas por 4 columnas que contiene las
 *   vocales de los 4 vectores (cada vector pasa a ser una columna).
 *
 * COMO FUNCIONA:
 *   - Rellena v1[5], v2[5], v3[5], v4[5] con vocales aleatorias.
 *   - Asigna m[i][0] = v1[i], m[i][1] = v2[i], m[i][2] = v3[i], m[i][3] = v4[i].
 *   - Muestra los 4 vectores y la matriz 5x4 resultante.
 *
 * TIPOS DE DATOS UTILIZADOS:
 *   - char: Vectores v1..v4[5], matriz m[5][4] y opcion de repeticion.
 *   - int : Indices de recorrido.
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
    char v1[5], v2[5], v3[5], v4[5];
    char m[5][4];
    int i, j;
    char repetir;

    srand((unsigned int)time(NULL));

    do {
        printf("\n=========================================================\n");
        printf("  EJERCICIO 54: 4 VECTORES DE VOCALES A MATRIZ DE 5x4\n");
        printf("=========================================================\n\n");

        for (i = 0; i < 5; i++) {
            v1[i] = vocales[rand() % 5];
            v2[i] = vocales[rand() % 5];
            v3[i] = vocales[rand() % 5];
            v4[i] = vocales[rand() % 5];

            m[i][0] = v1[i];
            m[i][1] = v2[i];
            m[i][2] = v3[i];
            m[i][3] = v4[i];
        }

        printf("Vectores de tamano 5 generados:\n\n");
        printf("   Vector 1: [");
        for (i = 0; i < 5; i++) printf(" %c ", v1[i]);
        printf("]\n");
        printf("   Vector 2: [");
        for (i = 0; i < 5; i++) printf(" %c ", v2[i]);
        printf("]\n");
        printf("   Vector 3: [");
        for (i = 0; i < 5; i++) printf(" %c ", v3[i]);
        printf("]\n");
        printf("   Vector 4: [");
        for (i = 0; i < 5; i++) printf(" %c ", v4[i]);
        printf("]\n");

        printf("\nMatriz resultante (5x4):\n\n");
        printf("         V1  V2  V3  V4\n");
        for (i = 0; i < 5; i++) {
            printf("Fila %d |", i + 1);
            for (j = 0; j < 4; j++) {
                printf("  %c ", m[i][j]);
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
