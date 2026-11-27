/* EJERCICIO 5: TABLERO DE AJEDREZ
   Funcionamiento: una matriz 8x8 se llena con puntos y despues se colocan las
   piezas: P peon, T torre, C caballo, A alfil, R rey y M reina.
   Tipos: char para la matriz y letras; int para indices.
*/
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    char tablero[8][8];
    int fila;
    int columna;

    for (fila = 0; fila < 8; fila++)
    {
        for (columna = 0; columna < 8; columna++)
        {
            tablero[fila][columna] = '.';
        }
    }

    tablero[0][0] = 'T'; tablero[0][1] = 'C';
    tablero[0][2] = 'A'; tablero[0][3] = 'M';
    tablero[0][4] = 'R'; tablero[0][5] = 'A';
    tablero[0][6] = 'C'; tablero[0][7] = 'T';
    tablero[7][0] = 'T'; tablero[7][1] = 'C';
    tablero[7][2] = 'A'; tablero[7][3] = 'M';
    tablero[7][4] = 'R'; tablero[7][5] = 'A';
    tablero[7][6] = 'C'; tablero[7][7] = 'T';

    for (columna = 0; columna < 8; columna++)
    {
        tablero[1][columna] = 'P';
        tablero[6][columna] = 'P';
    }

    printf("Tablero de ajedrez:\n\n");
    for (fila = 0; fila < 8; fila++)
    {
        for (columna = 0; columna < 8; columna++)
        {
            printf("%c ", tablero[fila][columna]);
        }
        printf("\n");
    }

    system("pause");
    return 0;
}
