#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <math.h>
/* 
ELABORADO 28/9/2026 -- JUAN MEDINA CI:31708027

4. Ingrese por teclado, los siguientes datos producto de una llamada telefónica: Nombre, Número de teléfono y cantidad de minutos de la llamada 
como VALOR ENTERO. SE PIDE: Calcular el costo de la llamada telefónica teniendo en cuenta que el costo por minuto es de 8$. Imprimir el nombre de 
la persona con el costo de la llamada respectiva.

*/

#define min 8

int main ()
{
    char nombre[100], numtlf[15];
    int minllamada;
    float total;

    printf ("CALCULADORA DE PROMEDIO DE 3 NOTAS\n");
    printf ("Ingrese su nombre: \n");
    scanf ("%s", &nombre);
    printf ("Ingrese su numero de telefono (+584XX-XXXXXXX ): \n");
    scanf ("%s", &numtlf);
    printf ("INGRESE LA CANTIDAD DE MINUTOS DE CONVERSACION: \n");
    scanf ("%d", &minllamada);
    
    total = minllamada*min;

    printf ("\nEl usuario %s y que tiene de numero %s tiene que pagar %.2f$", nombre, numtlf, total);
    getche();
}
