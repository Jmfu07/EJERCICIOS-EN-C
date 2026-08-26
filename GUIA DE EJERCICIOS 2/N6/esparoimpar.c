#define __USE_MINGW_ANSI_STDIO 1
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>

// ELABORADO POR JUAN MEDINA 16/08/2026

/*

leer un valor n cualquiera y decir si es par o impar

*/

void limpiarBuffer() 
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main ()
{
    int n;
    long double resultado;
    do 
    {
        system("cls");
        printf("Ingrese un numero: ");
        if (scanf("%d", &n) != 1) 
        {
            printf("Error: Dato invalido.\n");
            limpiarBuffer();
            sleep(3);
            continue;
        }
        limpiarBuffer();
        break;  

    } while (1);

    resultado = n%2;

    if (resultado == 0)
    {
        printf ("Es par");
    }
    else
    {
        printf ("Es impar");
    }

    sleep(10);
}