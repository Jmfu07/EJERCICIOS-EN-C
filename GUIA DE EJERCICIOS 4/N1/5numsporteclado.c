#define __USE_MINGW_ANSI_STDIO 1
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>

// ELABORADO POR JUAN MEDINA 20/08/2026

/*

Que lea 5 numeros por teclado, los copie en otro array multiplicados por 2 
y muestre el segundo array.

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

    int a [5], b[5], i=0;
    for (i = 0; i < 5; i++)
    {
        a[i]=validacion("Ingresa un numero: ");
        b[i]=a[i]*2;
    }


    for (i = 0; i < 5; i++)
    {
        printf ("%d ", a[i]);
    }

    for (i = 0; i < 5; i++)
    {
        printf ("%d ", b[i]);
    }
    sleep(10);
}