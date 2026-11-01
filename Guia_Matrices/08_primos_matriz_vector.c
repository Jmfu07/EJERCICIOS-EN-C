/* EJERCICIO 8: PRIMOS EN MATRIZ Y VECTOR
   Funcionamiento: cada numero se prueba desde 2 hasta su raiz cuadrada. Si no
   tiene divisores exactos, es primo y se incrementa el contador.
   Tipos: int para matriz, vector, indices y contadores.
*/
#include <stdio.h>
#include <stdlib.h>

int esPrimo(int numero)
{
    int divisor;
    if (numero < 2)
    {
        return 0;
    }
    for (divisor = 2; divisor * divisor <= numero; divisor++)
    {
        if (numero % divisor == 0)
        {
            return 0;
        }
    }
    return 1;
}

int main(void)
{
    int matriz[3][3];
    int vector[5];
    int cantidadPrimos = 0;
    int fila;
    int columna;
    int i;

    printf("Ingrese los 9 elementos de la matriz:\n");
    for (fila = 0; fila < 3; fila++)
    {
        for (columna = 0; columna < 3; columna++)
        {
            scanf("%d", &matriz[fila][columna]);
            if (esPrimo(matriz[fila][columna]))
            {
                cantidadPrimos++;
            }
        }
    }

    printf("Ingrese los 5 elementos del vector:\n");
    for (i = 0; i < 5; i++)
    {
        scanf("%d", &vector[i]);
        if (esPrimo(vector[i]))
        {
            cantidadPrimos++;
        }
    }

    printf("Cantidad total de primos: %d\n", cantidadPrimos);
    system("pause");
    return 0;
}
