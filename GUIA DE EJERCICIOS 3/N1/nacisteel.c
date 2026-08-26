#define __USE_MINGW_ANSI_STDIO 1
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>

// ELABORADO POR JUAN MEDINA 20/08/2026

/*

Escribir un programa que de un mensaje de saludo y luego muestre un mensaje que digga "Porfavor introduzca el año en que nacio"
luego el programa debe soltar ¨Si usted nacio en (fecha) este año cumple (edad calculada) años¨

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

    int e, r = 2026, resultado;

    printf("Hola! Que tal?");
    e = validacion("Ingresa el año en que nacio: ");
    resultado = r - e;
    printf ("Si usted nacio en %d usted tendra en %d la edad de %d años", e, r, resultado);
    sleep(5);
}