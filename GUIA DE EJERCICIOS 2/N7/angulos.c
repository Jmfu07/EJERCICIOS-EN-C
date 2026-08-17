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

int main ()
{
    double angulo;

    do 
    {
        system("cls");
        printf("Ingrese un angulo (deg): ");
        if (scanf("%d", &angulo) != 1 || angulo < 0) 
        {
            printf("Error: Dato invalido.\n");
            limpiarBuffer();
            sleep(3);
            continue;
        }
        limpiarBuffer();
        break;  

    } while (1);

    if (angulo == 90)
    {
        printf ("Angulo recto");
        sleep(5);
    }
    else if (angulo > 90)
    {
        printf ("Angulo obtuso");
        sleep(5);
    }
    else if (angulo < 90)
    {
        printf ("Angulo agudo");
        sleep(5);
    }

}