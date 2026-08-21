#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>

// ELABORADO POR JUAN MEDINA 20/08/2026

/*

calcule el area de un triangulo rectangulo, pidiendo al usuario la altura y la base.
- Valida si es un triangulo rectangulo
- formula: area = (base*altura)/2

*/

void limpiarBuffer() 
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    
}
