#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>

// ELABORADO POR JUAN MEDINA 20/08/2026

/*

Realizar un programa que tenga un array unidimensional de 20 elementos de tipo
numerico entero y de nombre "pares". Cargar el array con los primeros 20 numeros 
pares y mostrar el contenido del array en pantalla en grupos de 5.

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
