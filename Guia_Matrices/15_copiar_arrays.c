/* EJERCICIO 15: COPIA DE ARRAYS
   Funcionamiento: copiarArray recibe un arreglo de origen, otro de destino y
   el tamano. Cada elemento del origen se asigna al destino.
   Tipos: int[] para arreglos e int para tamano e indices.
*/
#include <stdio.h>
#include <stdlib.h>

void copiarArray(int origen[], int destino[], int tamano)
{
    int i;
    for (i = 0; i < tamano; i++)
    {
        destino[i] = origen[i];
    }
}

int main(void)
{
    int origen[10];
    int destino[10];
    int i;

    printf("Ingrese 10 elementos:\n");
    for (i = 0; i < 10; i++)
    {
        scanf("%d", &origen[i]);
    }

    copiarArray(origen, destino, 10);

    printf("Segundo arreglo:\n");
    for (i = 0; i < 10; i++)
    {
        printf("%d ", destino[i]);
    }
    printf("\n");
    system("pause");
    return 0;
}
