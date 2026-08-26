#define __USE_MINGW_ANSI_STDIO 1
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>

// ELABORADO POR JUAN MEDINA 20/08/2026

/*

calcule el area de un triangulo rectangulo, pidiendo al usuario la altura y la base.
- formula: area = (base*altura)/2

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
    double alt, base, area;

    alt = validacion("Ingrese la altura: ");
    base = validacion("Ingrese la base: ");

    area = (base*alt)/2;

    printf("El area es: %.2lf", area);
    sleep(10);

}