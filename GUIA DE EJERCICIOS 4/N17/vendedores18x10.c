#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>

// ELABORADO POR JUAN MEDINA 20/08/2026

/*

Vendedores. Crear un programa llamado vendedores que cree un array de 18x10 indicando que 
poseemos una empresa de 18 vendedores cada uno de los cuales vende 10 productos. El array 
almacena los ingresos obtenidos por cada vendedor en cada producto, de modo que un menu 
permite almacenar los ingresos, revisar el total de cada vendedor y obtener los ingresos 
totales.

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
