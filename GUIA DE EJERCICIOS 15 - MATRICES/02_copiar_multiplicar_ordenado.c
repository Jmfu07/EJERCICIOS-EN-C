/* EJERCICIO 2: COPIAR, MULTIPLICAR Y ORDENAR
   Funcionamiento: se leen cinco numeros, se multiplican por 2, se copian al
   tercer arreglo y se ordenan con el metodo de burbuja.
   Tipos: int para los arreglos, numeros, indices y auxiliar.
*/
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int original[5];
    int multiplicados[5];
    int ordenados[5];
    int auxiliar;
    int i;
    int j;

    printf("Ingrese 5 numeros enteros:\n");
    for (i = 0; i < 5; i++)
    {
        printf("Numero %d: ", i + 1);
        scanf("%d", &original[i]);
        multiplicados[i] = original[i] * 2;
        ordenados[i] = multiplicados[i];
    }

    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4 - i; j++)
        {
            if (ordenados[j] > ordenados[j + 1])
            {
                auxiliar = ordenados[j];
                ordenados[j] = ordenados[j + 1];
                ordenados[j + 1] = auxiliar;
            }
        }
    }

    printf("Segundo arreglo multiplicado por 2:\n");
    for (i = 0; i < 5; i++)
    {
        printf("%d ", multiplicados[i]);
    }
    printf("\nTercer arreglo ordenado:\n");
    for (i = 0; i < 5; i++)
    {
        printf("%d ", ordenados[i]);
    }
    printf("\n");
    system("pause");
    return 0;
}
