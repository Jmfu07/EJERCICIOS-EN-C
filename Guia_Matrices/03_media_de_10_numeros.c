/* EJERCICIO 3: MEDIA DE 10 NUMEROS
   Funcionamiento: se almacenan diez numeros, se suman y se divide entre 10.
   Tipos: double para numeros con decimales; int para indices.
*/
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    double numeros[10];
    double suma = 0.0;
    double media;
    int i;

    for (i = 0; i < 10; i++)
    {
        printf("Ingrese el numero %d: ", i + 1);
        scanf("%lf", &numeros[i]);
        suma = suma + numeros[i];
    }

    media = suma / 10.0;
    printf("Media: %.2f\n", media);
    system("pause");
    return 0;
}
