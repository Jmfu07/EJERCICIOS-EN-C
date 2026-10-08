#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <math.h>
/* 
ELABORADO 28/9/2026 -- JUAN MEDINA CI:31708027

5. Ingrese por teclado el radio de un círculo y calculé su área.

*/

#define PI 3.14

int main ()
{
    float radio, area;
    printf("INGRESE EL RADIO: \n");
    scanf("%f", &radio);

    area = PI * pow(radio,2);
    printf ("EL AREA DE UNA CIRCUNFERENCIA DE RADIO %.2f ES DE %.2f UNIDADES\n", radio, area);
    getche();
}
