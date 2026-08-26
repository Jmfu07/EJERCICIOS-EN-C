#define __USE_MINGW_ANSI_STDIO 1
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>

// ELABORADO POR JUAN MEDINA 20/08/2026

/*

Un programa que gestione las notas de una clase de 20 alumnos de los cuales sabemos el nombre y la nota.
El programa debe ser capaz de:
1- Buscar un alumno
2- Modificar su nota
3. Realizar la media de todas las notas
4. Realizar la media de las notas menores de 5
5. Mostrar el alumno que mejores notas ha sacado
6. Mostrar el alumno que peores notas ha sacado

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
