#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>

// ELABORADO POR JUAN MEDINA 16/08/2026

/*

Un angulo se considera agudo si es menor a 90 grados, obtuso si es mayor de 90 grados y 
recto si es igual a 90 grados

debemos pedir un angulo y el programa decoir cuando es recto, obtuso o agudo

*/

void limpiarBuffer() 
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}