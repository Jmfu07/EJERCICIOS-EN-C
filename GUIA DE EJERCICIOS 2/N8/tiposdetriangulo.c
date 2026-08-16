#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>

// ELABORADO POR JUAN MEDINA 16/08/2026

/*

Desarrollar un programa que lea 3 longitudes y determine si forman o no un triangulo
Debe decir el tipo de triangulo:
Equilatero (3 lados iguales), Isosceles (si tiene dos lados iguales) o Escaleno (si tiene
3 lados desiguales)

Para que un triangulo exista debe cumplir con que la suma del lado mayor sea menor que 
la suma de los otros dos lados

*/

void limpiarBuffer() 
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}