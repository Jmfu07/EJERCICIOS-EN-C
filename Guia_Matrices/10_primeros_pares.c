/* EJERCICIO 10: 20 PRIMEROS NUMEROS PARES
   Funcionamiento: el arreglo se carga con los valores 2, 4, 6 ... 40 y se
   imprime en grupos de cinco elementos.
   Tipos: int para arreglo, valores e indices.
*/
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int pares[20];
    int i;

    for (i = 0; i < 20; i++)
    {
        pares[i] = (i + 1) * 2;
    }

    for (i = 0; i < 20; i++)
    {
        printf("%d ", pares[i]);
        if ((i + 1) % 5 == 0)
        {
            printf("\n");
        }
    }

    system("pause");
    return 0;
}
