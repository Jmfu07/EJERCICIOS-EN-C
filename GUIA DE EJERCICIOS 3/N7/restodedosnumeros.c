#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>

// ELABORADO POR JUAN MEDINA 20/08/2026

/*

Escriba un programa que calcule el resto de la division de dos numeros enteros

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
        printf("Calculadora de residuo\n");
        printf("%s", mensaje);
        if (scanf("%lf", &num) != 1 || num <=0) 
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

int main ()
{
    double n1, n2, residuo;
    n1 = validacion("Ingrese el dividendo: ");
    n2 = validacion("Ingrese el divisor: ");

    // solo para numeros enteros residuo = n1 % n2;

    // Para variables double, float o long double:
    residuo = fmod(n1, n2);
    printf("El Residuo de esta division es: %.2lf", residuo);
    sleep(10);

}