/* EJERCICIO 11: POSITIVOS, NEGATIVOS Y CEROS
   Funcionamiento: se leen 30 enteros y se clasifican con tres contadores.
   Tipos: int para arreglo, valores, indices y contadores.
*/
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int numeros[30];
    int positivos = 0;
    int negativos = 0;
    int ceros = 0;
    int i;

    for (i = 0; i < 30; i++)
    {
        printf("Ingrese el numero %d: ", i + 1);
        scanf("%d", &numeros[i]);
        if (numeros[i] > 0)
        {
            positivos++;
        }
        else if (numeros[i] < 0)
        {
            negativos++;
        }
        else
        {
            ceros++;
        }
    }

    printf("Positivos: %d\n", positivos);
    printf("Negativos: %d\n", negativos);
    printf("Ceros: %d\n", ceros);
    system("pause");
    return 0;
}
