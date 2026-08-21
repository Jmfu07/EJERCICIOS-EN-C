#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>

// ELABORADO POR JUAN MEDINA 20/08/2026

/*

Escribir un programa que de un mensaje de saludo y luego muestre un mensaje que digga "Porfavor introduzca el año en que nacio"
luego el programa debe soltar ¨Si usted nacio en (fecha) este año cumple (edad calculada) años¨

*/

void limpiarBuffer() 
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    
}
