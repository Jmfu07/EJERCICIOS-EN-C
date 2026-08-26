#define __USE_MINGW_ANSI_STDIO 1
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>

// ELABORADO POR JUAN MEDINA 20/08/2026

/*

Un programa que lea 10 numeros por teclado, los almacene en un array y muestre la media

*/

void limpiarBuffer() 
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    
}

int validacion (const char *mensaje)
{
    int num;
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

int main ()
{

    int a [10], suma=0, i=0;
    for (i = 0; i < 10; i++)
    {
        a[i]=validacion("Ingresa un numero: ");
        suma += a[i];
    }


    for (i = 0; i < 10; i++)
    {
        printf ("%d ", a[i]);
    }
    printf ("\n La suma de todos es %d", suma);
    sleep(10);
}