#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>

// ELABORADO POR JUAN MEDINA 16/08/2026

/*

Desarrollar un programa que lea 3 longitudes y determine si forman o no un triangulo
Debe decir el tipo de triangulo:
Equilatero (3 lados iguales), Isosceles (si tiene dos lados iguales) o Escaleno (si tiene
3 lados desiguales)

Para que un triangulo exista debe cumplir con que la suma del lado mayor sea menor que 
la suma de los otros dos lados

*/

void limpiarBuffer() 
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

double validaciontriangulos (const char *mensaje)
{
    double lados;
    do 
    {
        system("cls");
        printf("%s", mensaje);
        if (scanf("%lf", &lados) != 1 || lados <= 0) 
        {
            printf("Error: Ingrese un numero entero positivo.\n");
            limpiarBuffer();
            sleep(3);
        } 
        else 
        {
            limpiarBuffer();
            return lados;
        }
    } while (1);

}

void estriangulo(double l1, double l2, double l3);
void estriangulo(double l1, double l2, double l3)
{
    int trt;
    double ladomayor, sumladosmenores;
 
    if ((l1 + l2 > l3) && (l1 + l3 > l2) && (l2 + l3 > l1))
    {
        printf("Resultado: Es un triangulo "); 

        if (l1 == l2 && l2 == l3)
        {
            printf("Equilatero.\n");
        }
        else if (l1 == l2 || l2 == l3 || l1 == l3)
        {
            printf("Isosceles.\n");
        }
        else
        {
            printf("Escaleno.\n");
        }
    }
    else
    {
        printf("Resultado: No es un triangulo.\n");
    }
    
    sleep(5);

}

int main ()
{
    double l1, l2, l3;
    double ladomayor, sumladosmenores;

    l1 = validaciontriangulos("Ingrese el Lado 1: ");
    l2 = validaciontriangulos("Ingrese el Lado 2: ");
    l3 = validaciontriangulos("Ingrese el Lado 3: ");

    printf ("Lado 1: %.2f\n", l1);
    printf ("Lado 2: %.2f\n", l2);
    printf ("Lado 3: %.2f\n", l3);
    
    estriangulo(l1,l2,l3);
}