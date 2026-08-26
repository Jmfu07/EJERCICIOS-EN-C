#define __USE_MINGW_ANSI_STDIO 1
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>

// ELABORADO POR JUAN MEDINA 20/08/2026

/*

Escribe un programa que declare 3 variables  a,b y c y se asigne a esas variables los 
valores 5, 7 y 9 respectivamente, estos valores deben sumarse y mostrarse. El programa 
debe pedir estos tres valores al usuario tambien. 


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
    int a= 5, b= 7, c= 9;
    int suma1, suma2;

    suma1 = a + b + c;
    printf ("La suma es: %d", suma1); sleep(5);

    a = validacion("Ingrese un numero: ");
    b = validacion("Ingrese un numero: ");
    c = validacion("Ingrese un numero: ");

    suma2= a + b + c;
    system("cls");
    printf ("La suma es: %d", suma2); sleep(5);
}
