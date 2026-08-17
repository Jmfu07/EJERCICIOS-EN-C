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
void limpiarbuffer();
void limpiarBuffer() 
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main ()
{
    double nota;

    do 
    {
        system("cls");
        printf("Ingrese su nota: ");
        if (scanf("%lf", &nota) != 1 || nota < 0 || nota > 10) 
        {
            printf("Error: Dato invalido, solo numeros mayores a cero.\n");
            limpiarBuffer();
            sleep(3);
            continue;
        }
        limpiarBuffer();
        break;

    } while (1);

    if (nota < 5.0)
    {
        printf ("SUSPENDIDO");
    }
    else if (nota < 7.0)
    {
        printf ("APROBADO");
    }
    else if (nota < 9.0)
    {
        printf ("NOTABLE");
    }
    else if (nota < 10)
    {
        printf ("SOBRESALIENTE");
    }
    else if (nota == 10)
    {
        printf ("MATRICULA DE HONOR");
    }

    sleep(5);
}