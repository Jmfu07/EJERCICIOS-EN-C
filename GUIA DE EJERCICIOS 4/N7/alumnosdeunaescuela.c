#define __USE_MINGW_ANSI_STDIO 1
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>

// ELABORADO POR JUAN MEDINA 20/08/2026

/*

Tenemos en una matriz los alumnos de una escuela. Sabiendo que hay 3 cursos distintos
5 alumnos por curso y que cada alumno tiene 2 asignaturas, se pide:

1. Pedir por teclado las notas que ha sacado cada alumno en cada asignatura y de cada curso
2. Decir cual es la nota media de un determinado curso
3. Decir cuantos aprobados y suspensos hay en una determinada asignatura en un determinado curso


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
