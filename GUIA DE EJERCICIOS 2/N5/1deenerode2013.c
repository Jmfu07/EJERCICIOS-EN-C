#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>

// ELABORADO POR JUAN MEDINA 16/08/2026

/*

El programa debe aceptar una fecha por teclado en el dia y mes correspondiente al 2013
el programa debe decir desde ese dia cuantos dias han pasado desde el primero de enero 
de 2013 (1 de enero de 2013)


*/

void limpiarBuffer() 
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main ()
{
    int year = 2013;
    int dias = 365;
    int enero = 31, febrero = 28, marzo = 31, abril = 30, mayo = 31, junio = 30;
    int julio = 31, agosto = 31, septiembre = 30, octubre = 31, noviembre = 30, diciembre = 31;

    

}