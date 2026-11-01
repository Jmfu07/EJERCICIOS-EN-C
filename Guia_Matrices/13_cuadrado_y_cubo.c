/* EJERCICIO 13: CUADRADO Y CUBO
   Funcionamiento: para cada entero se calcula numero*numero y luego el cubo.
   Se usa long long en los resultados para tener un rango mayor.
   Tipos: int para datos e indices; long long para cuadrados y cubos.
*/
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int numeros[10];
    long long cuadrado;
    long long cubo;
    int i;

    for (i = 0; i < 10; i++)
    {
        printf("Ingrese el numero %d: ", i + 1);
        scanf("%d", &numeros[i]);
    }

    printf("Numero\tCuadrado\tCubo\n");
    for (i = 0; i < 10; i++)
    {
        cuadrado = (long long) numeros[i] * numeros[i];
        cubo = cuadrado * numeros[i];
        printf("%d\t%lld\t\t%lld\n",
               numeros[i], cuadrado, cubo);
    }

    system("pause");
    return 0;
}
