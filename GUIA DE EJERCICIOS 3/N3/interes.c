#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>

// ELABORADO POR JUAN MEDINA 20/08/2026

/*

Escribe un programa que calcule el interes que se obtiene por un determinado dinero
depositado en un banco, el programa debe preguntar por el capital inicial y por el 
tipo de interes

*/

void limpiarBuffer() 
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    
}
