#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>

// ELABORADO POR JUAN MEDINA 20/08/2026

/*

Escribe un programa que declare 3 variables  a,b y c y se asigne a esas variables los 
valores 5, 7 y 9 respectivamente, estos valores deben sumarse y mostrarse. El programa 
debe pedir estos tres valores al usuario tambien. 


*/

void limpiarBuffer() 
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    
}
