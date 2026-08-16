#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>

// ELABORADO POR JUAN MEDINA 16/08/2026

/*
A un trabajador le pagan segun sus horas trabajadas y la tarifa esta a un valor por hora. si la cantidad 
de horas trabajadas es mayor a 40 horas, la tarifa por hora se incrementa en un 50% por las horas extra

Calcular el salario

*/

void limpiarBuffer() 
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main ()
{
    int tarifa;
    int horas, salario;

    do 
    {
        system("cls");
        printf("Ingrese la tarifa: ");
        if (scanf("%d", &tarifa) != 1 || tarifa <= 0) 
        {
            printf("Error: Dato invalido, solo numeros mayores a cero.\n");
            limpiarBuffer();
            sleep(3);
            continue;
        }
        limpiarBuffer();
        break;  

    } while (1);

    do 
    {
        system("cls");
        printf("Ingrese las horas trabajadas: ");
        if (scanf("%d", &horas) != 1 || horas <= 0) 
        {
            printf("Error: Dato invalido, solo numeros mayores a cero.\n");
            limpiarBuffer();
            sleep(3);
            continue;
        }
        limpiarBuffer();
        break;  

    } while (1);

    if (horas >= 40) 
    {
        salario = ((horas * tarifa)+(horas*tarifa*0.50));
        printf ("- Su salario es de %d", salario);
        sleep(5);
    }
    else
    {
        salario = horas * tarifa;
        printf ("- Su salario es de %d", salario);
        sleep(5);
    }
}