/* EJERCICIO 9: SUMA DE LAS DOS DIAGONALES
   Funcionamiento: en una matriz 20x20 la diagonal principal usa [i][i] y la
   secundaria usa [i][19-i]. Se suman ambas por separado y luego se combinan.
   Tipos: int para matriz, indices y sumas.
*/
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int matriz[20][20];
    int fila;
    int columna;
    int sumaPrincipal = 0;
    int sumaSecundaria = 0;

    printf("Ingrese los 400 elementos de la matriz 20x20:\n");
    for (fila = 0; fila < 20; fila++)
    {
        for (columna = 0; columna < 20; columna++)
        {
            scanf("%d", &matriz[fila][columna]);
        }
    }

    for (fila = 0; fila < 20; fila++)
    {
        sumaPrincipal = sumaPrincipal + matriz[fila][fila];
        sumaSecundaria = sumaSecundaria + matriz[fila][19 - fila];
    }

    printf("Diagonal principal: %d\n", sumaPrincipal);
    printf("Diagonal secundaria: %d\n", sumaSecundaria);
    printf("Suma de las dos diagonales: %d\n",
           sumaPrincipal + sumaSecundaria);

    system("pause");
    return 0;
}
