#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

// ELABORADO POR JUAN MEDINA 16/08/2026

/* 

*/

void limpiarBuffer() 
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}