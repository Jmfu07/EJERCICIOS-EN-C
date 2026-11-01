/* EJERCICIO 4: ORDENAR POSITIVOS Y NEGATIVOS
   Funcionamiento: se guardan los numeros y se ordenan de menor a mayor con
   burbuja. Los negativos quedan antes que los positivos.
   Tipos: int para cantidad, arreglo, indices y valores.
*/
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int numeros[100];
    int cantidad;
    int auxiliar;
    int i;
    int j;

    printf("Cantidad de numeros (1 a 100): ");
    scanf("%d", &cantidad);
    if (cantidad < 1 || cantidad > 100)
    {
        printf("Cantidad invalida.\n");
        system("pause");
        return 0;
    }

    for (i = 0; i < cantidad; i++)
    {
        printf("Numero %d: ", i + 1);
        scanf("%d", &numeros[i]);
    }

    for (i = 0; i < cantidad - 1; i++)
    {
        for (j = 0; j < cantidad - 1 - i; j++)
        {
            if (numeros[j] > numeros[j + 1])
            {
                auxiliar = numeros[j];
                numeros[j] = numeros[j + 1];
                numeros[j + 1] = auxiliar;
            }
        }
    }

    printf("Numeros ordenados:\n");
    for (i = 0; i < cantidad; i++)
    {
        printf("%d ", numeros[i]);
    }
    printf("\n");
    system("pause");
    return 0;
}
