#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>

// ELABORADO POR JUAN MEDINA 16/08/2026

/*

Un programa que pida una nota del 0 al 10 e imprima en pantalla SUSPENDIDO si la nota es menor que 5,
APROBADO si esta entre 5 y 7 sin incluir, NOTABLE si esta entre 7 y 9 sin incluir, SOBRESALIENTE si esta
entre 9 y 10 sin incluir y MATRICULA DE HONOR si la nota es igual a 10.

*/

void limpiarBuffer() 
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}