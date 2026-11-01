/* EJERCICIO 16: ORDENAR PARES E IMPARES
   Funcionamiento: se generan 100 numeros entre 1 y 1000. El ordenamiento de
   burbuja intercambia un impar seguido de un par, dejando los pares primero.
   Tipos: int para arreglo, valores, indices y auxiliar; time_t para aleatoriedad.
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void)
{
    int numeros[100];
    int auxiliar;
    int i;
    int j;

    srand((unsigned int) time(NULL));
    for (i = 0; i < 100; i++)
    {
        numeros[i] = rand() % 1000 + 1;
    }

    printf("Arreglo original:\n");
    for (i = 0; i < 100; i++)
    {
        printf("%d ", numeros[i]);
        if ((i + 1) % 10 == 0)
        {
            printf("\n");
        }
    }

    for (i = 0; i < 99; i++)
    {
        for (j = 0; j < 99 - i; j++)
        {
            if (numeros[j] % 2 != 0 && numeros[j + 1] % 2 == 0)
            {
                auxiliar = numeros[j];
                numeros[j] = numeros[j + 1];
                numeros[j + 1] = auxiliar;
            }
        }
    }

    printf("\nPares primero e impares despues:\n");
    for (i = 0; i < 100; i++)
    {
        printf("%d ", numeros[i]);
        if ((i + 1) % 10 == 0)
        {
            printf("\n");
        }
    }

    system("pause");
    return 0;
}
