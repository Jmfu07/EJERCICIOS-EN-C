/* EJERCICIO 1: COPIAR NUMEROS MULTIPLICADOS POR 2
   Funcionamiento: se leen cinco numeros y cada uno se copia al segundo arreglo
   despues de multiplicarlo por 2.
   Tipos: int para arreglos, valores e indices.
*/
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int original[5];
    int resultado[5];
    int i;

    printf("Ingrese 5 numeros enteros:\n");
    for (i = 0; i < 5; i++)
    {
        printf("Numero %d: ", i + 1);
        scanf("%d", &original[i]);
        resultado[i] = original[i] * 2;
    }

    printf("Segundo arreglo:\n");
    for (i = 0; i < 5; i++)
    {
        printf("%d ", resultado[i]);
    }
    printf("\n");
    system("pause");
    return 0;
}
