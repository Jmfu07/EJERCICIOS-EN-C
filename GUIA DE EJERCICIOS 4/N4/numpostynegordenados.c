#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>

// ELABORADO POR JUAN MEDINA 20/08/2026

/*

Mediante un array almacene numeros tanto positivos como negativos y los 
muestre ordenados.

*/

void limpiarBuffer() 
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    
}

double validacion (const char *mensaje)
{
    double num;
    do 
    {
        system("cls");
        printf("%s", mensaje);
        if (scanf("%d", &num) != 1 || num <=0) 
        {
            printf("Error: Ingrese un numero entero positivo.\n");
            limpiarBuffer();
            sleep(3);
        } 
        else 
        {
            limpiarBuffer();
            return num;
        }
    } while (1);

}
