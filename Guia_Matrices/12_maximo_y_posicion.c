/* EJERCICIO 12: MAXIMO Y POSICION
   Funcionamiento: se guardan 20 reales y se conserva el mayor. Solo se cambia
   el maximo con un valor estrictamente mayor, por lo que queda el primer indice
   cuando el maximo esta repetido.
   Tipos: double para valores; int para indices y posicion.
*/
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    double numeros[20];
    double maximo;
    int posicionMaximo;
    int i;

    for (i = 0; i < 20; i++)
    {
        printf("Ingrese el valor %d: ", i + 1);
        scanf("%lf", &numeros[i]);
    }

    maximo = numeros[0];
    posicionMaximo = 0;

    for (i = 1; i < 20; i++)
    {
        if (numeros[i] > maximo)
        {
            maximo = numeros[i];
            posicionMaximo = i;
        }
    }

    printf("Valor maximo: %.2f\n", maximo);
    printf("Indice: %d\n", posicionMaximo);
    printf("Posicion desde 1: %d\n", posicionMaximo + 1);
    system("pause");
    return 0;
}
